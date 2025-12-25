import { useState, useEffect, useRef } from "react";
import { Card, CardContent } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { ScrollArea } from "@/components/ui/scroll-area";
import { Input } from "@/components/ui/input";
import { Label } from "@/components/ui/label";
import { Tabs, TabsContent, TabsList, TabsTrigger } from "@/components/ui/tabs";
import { 
  Play, 
  Square, 
  Settings, 
  FileText, 
  Activity, 
  Database, 
  TrendingUp,
  Monitor,
  HardDrive,
  Plus,
  Trash2,
  Copy,
  ChevronDown,
  Info,
  History,
  AlertTriangle,
  Save,
  Download,
  Search,
  Users,
  BarChart3,
  RefreshCw,
  Maximize2
} from "lucide-react";
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  ResponsiveContainer,
  Legend,
  AreaChart,
  Area,
  ReferenceLine
} from "recharts";
import { toast } from "sonner";

// More realistic curve generation
const generateTestPoints = (maxPoints = 100) => {
  return Array.from({ length: maxPoints }, (_, i) => {
    const strain = i * 0.2;
    // Simple elastic-plastic model: stress = E * strain until yield, then hardening
    let stress = 0;
    const E = 1500; // Modulus
    const yieldStrain = 2;
    if (strain < yieldStrain) {
      stress = E * strain;
    } else {
      stress = E * yieldStrain + 200 * Math.pow(strain - yieldStrain, 0.5);
    }
    // Add noise
    stress += (Math.random() - 0.5) * 20;
    return {
      extension: strain.toFixed(2),
      stress: Math.max(0, stress).toFixed(2),
      force: (stress * 0.1).toFixed(2),
      strain: strain.toFixed(2)
    };
  });
};

const historicalData = [
  { batch: "B-202", avgStress: 45.2, date: "2025-12-20" },
  { batch: "B-203", avgStress: 48.7, date: "2025-12-21" },
  { batch: "B-204", avgStress: 42.1, date: "2025-12-22" },
  { batch: "B-205", avgStress: 50.3, date: "2025-12-23" },
];

export default function HorizonDashboard() {
  const [activeTab, setActiveTab] = useState("test");
  const [isRunning, setIsRunning] = useState(false);
  const [currentData, setCurrentData] = useState<any[]>([]);
  const [liveMetrics, setLiveMetrics] = useState({
    force: 0,
    extension: 0,
    stress: 0,
    strain: 0
  });
  
  const [samples, setSamples] = useState([
    { id: 1, name: "Sample A-101", status: "Ready", result: "-", width: 10, thickness: 4 },
    { id: 2, name: "Sample A-102", status: "Ready", result: "-", width: 10, thickness: 4 },
  ]);
  const [activeSampleId, setActiveSampleId] = useState(1);
  const [machineStatus, setMachineStatus] = useState("Online");
  const [loadCell, setLoadCell] = useState("STDM-100kN");
  const [testMethod, setTestMethod] = useState("Tensile ISO 527-2");
  
  const timerRef = useRef<NodeJS.Timeout | null>(null);
  const dataCounterRef = useRef(0);
  const fullCurveRef = useRef(generateTestPoints(200));

  const activeSample = samples.find(s => s.id === activeSampleId) || samples[0];

  useEffect(() => {
    if (isRunning) {
      dataCounterRef.current = 0;
      setCurrentData([]);
      fullCurveRef.current = generateTestPoints(150);
      
      timerRef.current = setInterval(() => {
        if (dataCounterRef.current < fullCurveRef.current.length) {
          const point = fullCurveRef.current[dataCounterRef.current];
          setCurrentData(prev => [...prev, point]);
          setLiveMetrics({
            force: parseFloat(point.force),
            extension: parseFloat(point.extension),
            stress: parseFloat(point.stress),
            strain: parseFloat(point.strain)
          });
          dataCounterRef.current++;
          
          // Auto-stop at break point (simulated end of array)
          if (dataCounterRef.current === fullCurveRef.current.length) {
            handleStop(true);
          }
        }
      }, 50);
    } else {
      if (timerRef.current) clearInterval(timerRef.current);
    }
    return () => { if (timerRef.current) clearInterval(timerRef.current); };
  }, [isRunning]);

  const handleStart = () => {
    if (activeSample.status === "Completed") {
      toast.info("Overwriting previous results for this sample");
    }
    setIsRunning(true);
    toast.success(`Starting ${testMethod} on ${activeSample.name}`);
  };

  const handleStop = (completed = false) => {
    setIsRunning(false);
    if (timerRef.current) clearInterval(timerRef.current);
    
    if (completed) {
      const peakStress = Math.max(...currentData.map(d => parseFloat(d.stress)));
      setSamples(prev => prev.map(s => 
        s.id === activeSampleId ? { ...s, status: "Completed", result: peakStress.toFixed(2) + " MPa" } : s
      ));
      toast.success("Test completed successfully");
    } else {
      toast.error("Test aborted");
    }
  };

  const updateSampleParam = (id: number, field: string, value: string) => {
    setSamples(prev => prev.map(s => s.id === id ? { ...s, [field]: parseFloat(value) || 0 } : s));
  };

  const addSample = () => {
    const newId = Math.max(...samples.map(s => s.id)) + 1;
    setSamples([...samples, { id: newId, name: `Sample A-${100 + newId}`, status: "Ready", result: "-", width: 10, thickness: 4 }]);
  };

  const deleteSample = (id: number) => {
    if (samples.length > 1) {
      setSamples(samples.filter(s => s.id !== id));
      if (activeSampleId === id) setActiveSampleId(samples[0].id);
    }
  };

  return (
    <div className="flex h-screen w-full bg-[#0f172a] text-slate-200 font-sans overflow-hidden select-none">
      {/* Sidebar - Tinius Olsen style */}
      <aside className="w-16 bg-[#020617] flex flex-col items-center py-4 gap-6 border-r border-slate-800 z-30">
        <div className="w-10 h-10 bg-blue-600 rounded flex items-center justify-center text-white font-black text-xl mb-4 shadow-lg shadow-blue-900/20">
          TO
        </div>
        <NavItem icon={<Activity size={22} />} active={activeTab === "test"} onClick={() => setActiveTab("test")} label="Test Center" />
        <NavItem icon={<Database size={22} />} active={activeTab === "data"} onClick={() => setActiveTab("data")} label="Batch Data" />
        <NavItem icon={<FileText size={22} />} active={activeTab === "reports"} onClick={() => setActiveTab("reports")} label="Report Center" />
        <NavItem icon={<Settings size={22} />} active={activeTab === "settings"} onClick={() => setActiveTab("settings")} label="Configuration" />
        <div className="mt-auto flex flex-col gap-4">
          <NavItem icon={<RefreshCw size={22} />} active={false} onClick={() => window.location.reload()} label="Refresh System" />
        </div>
      </aside>

      {/* Main Content */}
      <main className="flex-1 flex flex-col relative overflow-hidden bg-slate-950">
        {/* Pro Header */}
        <header className="h-12 bg-[#1e293b] border-b border-slate-700 flex items-center justify-between px-4 shadow-md z-20">
          <div className="flex items-center gap-6">
            <div className="flex items-center gap-2">
              <span className="text-blue-400 font-black italic tracking-tighter text-xl">HORIZON</span>
              <div className="h-4 w-[1px] bg-slate-600" />
              <span className="text-xs font-bold text-slate-400 uppercase tracking-widest">{testMethod}</span>
            </div>
          </div>
          <div className="flex items-center gap-4">
            <div className="flex items-center gap-4 px-4 py-1 bg-slate-900/50 rounded-full border border-slate-700 text-[10px] font-bold">
              <div className="flex items-center gap-1.5">
                <div className="w-1.5 h-1.5 rounded-full bg-blue-500 animate-pulse" />
                <span className="text-slate-300">CELL: {loadCell}</span>
              </div>
              <div className="w-[1px] h-3 bg-slate-700" />
              <div className="flex items-center gap-1.5">
                <div className={`w-1.5 h-1.5 rounded-full ${machineStatus === 'Online' ? 'bg-green-500' : 'bg-red-500'}`} />
                <span className={machineStatus === 'Online' ? 'text-green-400' : 'text-red-400'}>{machineStatus.toUpperCase()}</span>
              </div>
            </div>
            <Button variant="ghost" size="sm" className="h-8 text-xs text-slate-400 hover:text-white"><Save size={14} className="mr-2" /> Save</Button>
            <Button variant="ghost" size="sm" className="h-8 text-xs text-slate-400 hover:text-white"><Download size={14} className="mr-2" /> Export</Button>
          </div>
        </header>

        <div className="flex-1 flex overflow-hidden">
          {activeTab === "test" && (
            <>
              {/* Batch Sidebar */}
              <div className="w-72 bg-[#1e293b] border-r border-slate-700 flex flex-col">
                <div className="p-3 border-b border-slate-700 flex items-center justify-between bg-slate-900/30">
                  <h2 className="text-[10px] font-black uppercase tracking-tighter text-slate-400">Current Batch</h2>
                  <Button variant="ghost" size="icon" className="h-6 w-6 hover:bg-blue-600 hover:text-white" onClick={addSample}><Plus size={14} /></Button>
                </div>
                <ScrollArea className="flex-1">
                  <div className="p-2 space-y-1">
                    {samples.map(sample => (
                      <div 
                        key={sample.id}
                        onClick={() => setActiveSampleId(sample.id)}
                        className={`group p-2.5 rounded border transition-all cursor-pointer relative ${
                          activeSampleId === sample.id 
                            ? 'bg-blue-600/20 border-blue-500/50 ring-1 ring-blue-500/20' 
                            : 'bg-slate-800/40 border-slate-700 hover:border-slate-500'
                        }`}
                      >
                        <div className="flex justify-between items-start mb-1">
                          <span className={`text-xs font-bold ${activeSampleId === sample.id ? 'text-blue-300' : 'text-slate-300'}`}>{sample.name}</span>
                          <span className={`text-[8px] px-1.5 py-0.5 rounded-full font-black uppercase ${
                            sample.status === 'Completed' ? 'bg-green-500/20 text-green-400 border border-green-500/30' : 'bg-slate-700 text-slate-400'
                          }`}>{sample.status}</span>
                        </div>
                        <div className="text-[10px] text-slate-500 flex justify-between items-center">
                          <span>Peak: {sample.result}</span>
                          <button 
                            className="opacity-0 group-hover:opacity-100 p-1 hover:text-red-400 transition-opacity"
                            onClick={(e) => { e.stopPropagation(); deleteSample(sample.id); }}
                          >
                            <Trash2 size={10} />
                          </button>
                        </div>
                      </div>
                    ))}
                  </div>
                </ScrollArea>
                
                <div className="p-4 bg-slate-900/50 border-t border-slate-700 space-y-4">
                  <h2 className="text-[10px] font-black uppercase tracking-tighter text-slate-400">Sample Specs</h2>
                  <div className="grid grid-cols-2 gap-3">
                    <div className="space-y-1.5">
                      <Label className="text-[9px] font-bold text-slate-500 uppercase">Width (mm)</Label>
                      <Input 
                        value={activeSample.width} 
                        onChange={(e) => updateSampleParam(activeSampleId, 'width', e.target.value)}
                        className="h-7 bg-slate-800 border-slate-700 text-xs font-mono focus:ring-blue-500" 
                      />
                    </div>
                    <div className="space-y-1.5">
                      <Label className="text-[9px] font-bold text-slate-500 uppercase">Thickness (mm)</Label>
                      <Input 
                        value={activeSample.thickness} 
                        onChange={(e) => updateSampleParam(activeSampleId, 'thickness', e.target.value)}
                        className="h-7 bg-slate-800 border-slate-700 text-xs font-mono focus:ring-blue-500" 
                      />
                    </div>
                  </div>
                  <div className="p-2 bg-blue-500/10 rounded border border-blue-500/20">
                    <div className="flex justify-between text-[10px] font-bold text-blue-400 uppercase">
                      <span>Area</span>
                      <span>{(activeSample.width * activeSample.thickness).toFixed(2)} mm²</span>
                    </div>
                  </div>
                </div>
              </div>

              {/* Main Test Interface */}
              <div className="flex-1 flex flex-col relative">
                {/* Big Live Meters */}
                <div className="h-32 bg-[#020617] flex border-b border-slate-800 divide-x divide-slate-800 shadow-xl">
                  <BigMeter label="Force" value={liveMetrics.force.toFixed(2)} unit="N" color="text-blue-500" glow="shadow-blue-500/10" />
                  <BigMeter label="Extension" value={liveMetrics.extension.toFixed(3)} unit="mm" color="text-emerald-500" glow="shadow-emerald-500/10" />
                  <BigMeter label="Stress" value={liveMetrics.stress.toFixed(2)} unit="MPa" color="text-amber-500" glow="shadow-amber-500/10" />
                  <BigMeter label="Strain" value={liveMetrics.strain.toFixed(2)} unit="%" color="text-purple-500" glow="shadow-purple-500/10" />
                  
                  <div className="flex-1 flex items-center justify-center p-4 bg-slate-900/20">
                    <Button 
                      onClick={isRunning ? () => handleStop(false) : handleStart}
                      className={`w-48 h-16 text-xl font-black gap-3 shadow-2xl transition-all active:scale-95 ${
                        isRunning 
                          ? 'bg-red-600 hover:bg-red-700 shadow-red-900/20' 
                          : 'bg-green-600 hover:bg-green-700 shadow-green-900/20'
                      }`}
                    >
                      {isRunning ? <Square size={24} fill="currentColor" /> : <Play size={24} fill="currentColor" />}
                      {isRunning ? "HALT" : "START"}
                    </Button>
                  </div>
                </div>

                <div className="flex-1 p-4 grid grid-rows-[1fr_180px] gap-4">
                  {/* Test Curve */}
                  <Card className="bg-[#1e293b] border-slate-700 shadow-2xl flex flex-col relative overflow-hidden group">
                    <div className="px-4 py-2 border-b border-slate-700 flex justify-between items-center bg-slate-900/40">
                      <div className="flex gap-4">
                        <span className="text-[10px] font-black uppercase text-blue-400">Stress vs Strain Real-time</span>
                      </div>
                      <div className="flex gap-2">
                        <button className="text-slate-500 hover:text-white"><Maximize2 size={14} /></button>
                      </div>
                    </div>
                    <div className="flex-1 p-4">
                      <ResponsiveContainer width="100%" height="100%">
                        <LineChart data={currentData}>
                          <CartesianGrid strokeDasharray="3 3" vertical={false} stroke="#334155" />
                          <XAxis 
                            dataKey="strain" 
                            type="number" 
                            domain={[0, 'auto']} 
                            tick={{fontSize: 9, fill: '#64748b'}} 
                            label={{ value: 'Strain (%)', position: 'bottom', fontSize: 10, fill: '#475569' }}
                          />
                          <YAxis 
                            tick={{fontSize: 9, fill: '#64748b'}} 
                            label={{ value: 'Stress (MPa)', angle: -90, position: 'left', fontSize: 10, fill: '#475569' }}
                          />
                          <Tooltip 
                            contentStyle={{ backgroundColor: '#020617', border: '1px solid #334155', borderRadius: '4px', fontSize: '10px' }}
                            itemStyle={{ color: '#3b82f6' }}
                          />
                          <Line 
                            type="monotone" 
                            dataKey="stress" 
                            stroke="#3b82f6" 
                            strokeWidth={3} 
                            dot={false} 
                            isAnimationActive={false} 
                            shadow="0 0 10px #3b82f6"
                          />
                          <ReferenceLine y={0} stroke="#475569" />
                        </LineChart>
                      </ResponsiveContainer>
                    </div>
                    {/* Floating status */}
                    {isRunning && (
                      <div className="absolute top-12 right-6 bg-blue-600 px-3 py-1 rounded-full text-[10px] font-black animate-pulse flex items-center gap-2">
                        <div className="w-1.5 h-1.5 bg-white rounded-full" />
                        ACQUIRING DATA...
                      </div>
                    )}
                  </Card>

                  {/* Pro Results Grid */}
                  <div className="grid grid-cols-4 gap-4">
                    <ValueCard label="Yield Stress" value={isRunning ? (liveMetrics.stress * 0.85).toFixed(1) : "38.4"} unit="MPa" />
                    <ValueCard label="Peak Load" value={isRunning ? (liveMetrics.force).toFixed(2) : "485.2"} unit="N" />
                    <ValueCard label="Young's Modulus" value="1.42" unit="GPa" trend="+0.05" />
                    <ValueCard label="Energy to Break" value="12.4" unit="J" color="text-emerald-400" />
                  </div>
                </div>
              </div>

              {/* Hardware Sidebar */}
              <div className="w-64 bg-[#1e293b] border-l border-slate-700 flex flex-col">
                <div className="p-4 border-b border-slate-700">
                  <h2 className="text-[10px] font-black uppercase tracking-tighter text-slate-400 mb-4">Manual Controls</h2>
                  <div className="grid grid-cols-2 gap-2">
                    <HardwareBtn icon={<ChevronDown size={16} />} label="Jog Down" onClick={() => toast.info("Manual Jog: Down")} />
                    <HardwareBtn icon={<ChevronDown className="rotate-180" size={16} />} label="Jog Up" onClick={() => toast.info("Manual Jog: Up")} />
                    <HardwareBtn icon={<Monitor size={16} />} label="Zero Load" onClick={() => { setLiveMetrics(m => ({...m, force: 0})); toast.success("Load Cell Zeroed"); }} />
                    <HardwareBtn icon={<Monitor size={16} />} label="Zero Ext" onClick={() => { setLiveMetrics(m => ({...m, extension: 0})); toast.success("Extension Zeroed"); }} />
                  </div>
                </div>
                
                <div className="p-4 flex-1 space-y-4">
                  <h2 className="text-[10px] font-black uppercase tracking-tighter text-slate-400">Internal Sequence</h2>
                  <div className="space-y-3">
                    <StatusStep label="Grip Engagement" done />
                    <StatusStep label="Slack Removal" done />
                    <StatusStep label="Main Test Path" active={isRunning} />
                    <StatusStep label="Data Buffering" active={isRunning} />
                    <StatusStep label="Peak Detection" />
                  </div>
                </div>

                <div className="p-4 bg-red-950/30 border-t border-slate-700">
                  <Button 
                    variant="destructive" 
                    className="w-full h-14 bg-red-600 hover:bg-red-700 text-white font-black uppercase tracking-tighter flex flex-col gap-0 shadow-lg shadow-red-900/20"
                    onClick={() => handleStop(false)}
                  >
                    <span>EMERGENCY</span>
                    <span className="text-[10px] opacity-70">STOP SYSTEM</span>
                  </Button>
                </div>
              </div>
            </>
          )}

          {activeTab === "data" && (
            <div className="flex-1 p-8 bg-slate-950 overflow-auto">
              <div className="max-w-6xl mx-auto space-y-8">
                <div className="flex justify-between items-end">
                  <div>
                    <h2 className="text-3xl font-black text-white tracking-tighter">BATCH ANALYSIS</h2>
                    <p className="text-slate-500 font-medium">LIMS Integrated Database Results</p>
                  </div>
                </div>

                <div className="grid grid-cols-4 gap-4">
                  <StatBox label="Tests Run" value="1,248" />
                  <StatBox label="Pass Rate" value="98.2%" color="text-green-400" />
                  <StatBox label="Mean Stress" value="46.5" unit="MPa" />
                  <StatBox label="Std Dev" value="1.24" />
                </div>

                <Card className="bg-[#1e293b] border-slate-700 overflow-hidden">
                  <table className="w-full text-left">
                    <thead className="bg-slate-900/50 text-[10px] font-black uppercase text-slate-500 border-b border-slate-700">
                      <tr>
                        <th className="px-6 py-4">Batch ID</th>
                        <th className="px-6 py-4">Mean Strength</th>
                        <th className="px-6 py-4">Timestamp</th>
                        <th className="px-6 py-4 text-right">Records</th>
                      </tr>
                    </thead>
                    <tbody className="divide-y divide-slate-800">
                      {historicalData.map(item => (
                        <tr key={item.batch} className="hover:bg-blue-600/5 transition-colors cursor-pointer group">
                          <td className="px-6 py-4 text-xs font-bold text-slate-300">{item.batch}</td>
                          <td className="px-6 py-4 text-xs font-mono text-blue-400">{item.avgStress} MPa</td>
                          <td className="px-6 py-4 text-[10px] font-bold text-slate-500">{item.date}</td>
                          <td className="px-6 py-4 text-right">
                            <Button variant="ghost" size="sm" className="h-7 text-[10px] font-black text-blue-500 group-hover:bg-blue-600 group-hover:text-white">RECALL</Button>
                          </td>
                        </tr>
                      ))}
                    </tbody>
                  </table>
                </Card>
              </div>
            </div>
          )}

          {activeTab === "settings" && (
            <div className="flex-1 p-8 bg-slate-950 overflow-auto">
              <div className="max-w-4xl mx-auto space-y-12">
                <section className="bg-[#1e293b] p-8 rounded-xl border border-slate-700 shadow-2xl">
                  <h2 className="text-xl font-black text-white mb-8 tracking-tight">SYSTEM CONFIGURATION</h2>
                  <div className="grid grid-cols-2 gap-12">
                    <div className="space-y-6">
                      <div className="space-y-2">
                        <Label className="text-[10px] font-black text-slate-500 uppercase">Hardware Status</Label>
                        <select 
                          className="w-full h-10 bg-slate-900 border-slate-700 rounded-md px-3 text-xs font-bold text-slate-300 outline-none focus:ring-2 ring-blue-500"
                          value={machineStatus}
                          onChange={(e) => setMachineStatus(e.target.value)}
                        >
                          <option>Online</option>
                          <option>Calibration Mode</option>
                          <option>Service Required</option>
                        </select>
                      </div>
                      <div className="space-y-2">
                        <Label className="text-[10px] font-black text-slate-500 uppercase">Load Cell Registry</Label>
                        <select 
                          className="w-full h-10 bg-slate-900 border-slate-700 rounded-md px-3 text-xs font-bold text-slate-300 outline-none focus:ring-2 ring-blue-500"
                          value={loadCell}
                          onChange={(e) => setLoadCell(e.target.value)}
                        >
                          <option>STDM-100kN (#8821)</option>
                          <option>STDM-50kN (#4412)</option>
                          <option>STDM-1kN (#2102)</option>
                        </select>
                      </div>
                    </div>
                    <div className="p-6 bg-slate-900/50 rounded-xl border border-slate-800 space-y-4">
                      <h3 className="text-xs font-black text-blue-500 uppercase">CALIBRATION DATA</h3>
                      <div className="space-y-3">
                        <div className="flex justify-between text-[10px] font-bold"><span className="text-slate-500">LAST SYNC</span><span className="text-slate-300">2025-11-12</span></div>
                        <div className="flex justify-between text-[10px] font-bold"><span className="text-slate-500">COMPLIANCE</span><span className="text-green-400">ISO 7500-1</span></div>
                        <div className="h-[1px] bg-slate-800" />
                        <Button variant="outline" className="w-full h-8 text-[10px] font-black border-slate-700">RUN DIAGNOSTICS</Button>
                      </div>
                    </div>
                  </div>
                </section>
              </div>
            </div>
          )}
        </div>
      </main>
    </div>
  );
}

function NavItem({ icon, active, onClick, label }: { icon: React.ReactNode; active: boolean; onClick: () => void; label: string }) {
  return (
    <button
      onClick={onClick}
      className={`w-12 h-12 rounded-xl flex items-center justify-center transition-all group relative ${
        active ? "bg-blue-600 text-white shadow-xl shadow-blue-900/20 scale-110" : "text-slate-500 hover:bg-slate-800 hover:text-slate-300"
      }`}
    >
      {icon}
      <div className="absolute left-16 bg-[#020617] text-white text-[9px] px-2 py-1.5 rounded font-black opacity-0 group-hover:opacity-100 transition-all transform translate-x-[-10px] group-hover:translate-x-0 whitespace-nowrap pointer-events-none uppercase tracking-widest border border-slate-800 z-50 shadow-2xl">
        {label}
      </div>
    </button>
  );
}

function BigMeter({ label, value, unit, color, glow }: { label: string; value: string; unit: string; color: string; glow: string }) {
  return (
    <div className={`flex flex-col justify-center px-8 border-r border-slate-800 transition-colors ${glow}`}>
      <span className="text-[9px] font-black text-slate-500 uppercase mb-1 tracking-tighter">{label}</span>
      <div className="flex items-baseline gap-2">
        <span className={`text-4xl font-mono font-black tabular-nums ${color}`}>{value}</span>
        <span className="text-xs font-black text-slate-600 uppercase">{unit}</span>
      </div>
    </div>
  );
}

function ValueCard({ label, value, unit, trend, color = "text-white" }: { label: string; value: string; unit: string; trend?: string; color?: string }) {
  return (
    <div className="bg-[#1e293b] p-4 rounded-xl border border-slate-700 shadow-xl group hover:border-blue-500/50 transition-all">
      <div className="flex justify-between items-start mb-2">
        <span className="text-[9px] font-black text-slate-500 uppercase tracking-widest group-hover:text-blue-400">{label}</span>
        {trend && <span className="text-[10px] font-black text-green-400">+{trend}</span>}
      </div>
      <div className="flex items-baseline gap-2">
        <span className={`text-2xl font-black tabular-nums tracking-tighter ${color}`}>{value}</span>
        <span className="text-[10px] font-black text-slate-600">{unit}</span>
      </div>
    </div>
  );
}

function HardwareBtn({ icon, label, onClick }: { icon: React.ReactNode; label: string; onClick: () => void }) {
  return (
    <Button 
      variant="outline" 
      onClick={onClick}
      className="flex flex-col h-16 gap-1.5 bg-slate-900 border-slate-700 hover:bg-blue-600 hover:border-blue-500 hover:text-white transition-all group shadow-inner"
    >
      {icon}
      <span className="text-[8px] font-black uppercase text-slate-500 group-hover:text-white">{label}</span>
    </Button>
  );
}

function StatusStep({ label, done, active }: { label: string; done?: boolean; active?: boolean }) {
  return (
    <div className="flex items-center gap-3 group">
      <div className={`w-2 h-2 rounded-full border ${
        done ? 'bg-green-500 border-green-400' : 
        active ? 'bg-blue-500 border-blue-400 animate-pulse ring-2 ring-blue-500/20' : 
        'bg-slate-800 border-slate-700 group-hover:border-slate-500'
      }`} />
      <span className={`text-[10px] font-bold uppercase tracking-tight ${
        done ? 'text-slate-400' : 
        active ? 'text-blue-400' : 
        'text-slate-600'
      }`}>{label}</span>
    </div>
  );
}

function StatBox({ label, value, unit, color = "text-white" }: { label: string; value: string; unit?: string; color?: string }) {
  return (
    <div className="bg-[#1e293b] p-6 rounded-2xl border border-slate-700 shadow-xl">
      <span className="text-[10px] font-black text-slate-500 uppercase tracking-widest block mb-2">{label}</span>
      <div className="flex items-baseline gap-1">
        <span className={`text-4xl font-black tracking-tighter ${color}`}>{value}</span>
        {unit && <span className="text-xs font-black text-slate-600">{unit}</span>}
      </div>
    </div>
  );
}
