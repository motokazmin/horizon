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
  Plus,
  Trash2,
  ChevronDown,
  RefreshCw,
  Maximize2,
  Save,
  Download
} from "lucide-react";
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  ResponsiveContainer,
  ReferenceLine
} from "recharts";
import { toast } from "sonner";

// More realistic curve generation
const generateTestPoints = (maxPoints = 100) => {
  return Array.from({ length: maxPoints }, (_, i) => {
    const strain = i * 0.2;
    let stress = 0;
    const E = 1500; 
    const yieldStrain = 2;
    if (strain < yieldStrain) {
      stress = E * strain;
    } else {
      stress = E * yieldStrain + 200 * Math.pow(strain - yieldStrain, 0.5);
    }
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
  const [loadCell] = useState("STDM-100kN");
  const [testMethod] = useState("Tensile ISO 527-2");
  
  // Sequence Tracking
  const [sequenceStatus, setSequenceStatus] = useState({
    grip: "Done",
    slack: "Done",
    main: "Ready",
    buffer: "Ready",
    peak: "Ready"
  });

  const timerRef = useRef<NodeJS.Timeout | null>(null);
  const dataCounterRef = useRef(0);
  const fullCurveRef = useRef(generateTestPoints(150));

  const activeSample = samples.find(s => s.id === activeSampleId) || samples[0];

  useEffect(() => {
    if (isRunning) {
      dataCounterRef.current = 0;
      setCurrentData([]);
      fullCurveRef.current = generateTestPoints(150);
      setSequenceStatus(prev => ({ ...prev, main: "Active", buffer: "Active", peak: "Ready" }));
      
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
    if (machineStatus !== "Online") {
      toast.error("Machine must be ONLINE to start test");
      return;
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
      setSequenceStatus(prev => ({ ...prev, main: "Done", buffer: "Done", peak: "Done" }));
      toast.success("Test completed and results recorded");
    } else {
      setSequenceStatus(prev => ({ ...prev, main: "Ready", buffer: "Ready", peak: "Ready" }));
      toast.error("Test aborted");
    }
  };

  const handleExport = () => {
    const reportData = {
      method: testMethod,
      sample: activeSample.name,
      result: activeSample.result,
      timestamp: new Date().toLocaleString()
    };
    
    // Simulate file generation
    const blob = new Blob([JSON.stringify(reportData, null, 2)], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `Horizon_Report_${activeSample.name.replace(' ', '_')}.json`;
    a.click();
    toast.success("Report exported to JSON");
  };

  const handleJog = (dir: string) => {
    if (isRunning) {
      toast.warning("Cannot JOG while test is running");
      return;
    }
    const val = dir === 'up' ? 0.05 : -0.05;
    setLiveMetrics(prev => ({
      ...prev,
      extension: parseFloat((prev.extension + val).toFixed(3))
    }));
    toast.info(`Jogging ${dir.toUpperCase()}`);
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

      <main className="flex-1 flex flex-col relative overflow-hidden bg-slate-950">
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
            <Button variant="ghost" size="sm" className="h-8 text-xs text-slate-400 hover:text-white" onClick={() => toast.success("Batch saved to memory")}><Save size={14} className="mr-2" /> Save</Button>
            <Button variant="ghost" size="sm" className="h-8 text-xs text-slate-400 hover:text-white" onClick={handleExport}><Download size={14} className="mr-2" /> Export</Button>
          </div>
        </header>

        <div className="flex-1 flex overflow-hidden">
          {activeTab === "test" && (
            <>
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

              <div className="flex-1 flex flex-col relative">
                <div className="h-32 bg-[#020617] flex border-b border-slate-800 divide-x divide-slate-800 shadow-xl">
                  <BigMeter label="Force" value={liveMetrics.force.toFixed(2)} unit="N" color="text-blue-500" />
                  <BigMeter label="Extension" value={liveMetrics.extension.toFixed(3)} unit="mm" color="text-emerald-500" />
                  <BigMeter label="Stress" value={liveMetrics.stress.toFixed(2)} unit="MPa" color="text-amber-500" />
                  <BigMeter label="Strain" value={liveMetrics.strain.toFixed(2)} unit="%" color="text-purple-500" />
                  
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
                          />
                          <ReferenceLine y={0} stroke="#475569" />
                        </LineChart>
                      </ResponsiveContainer>
                    </div>
                    {isRunning && (
                      <div className="absolute top-12 right-6 bg-blue-600 px-3 py-1 rounded-full text-[10px] font-black animate-pulse flex items-center gap-2">
                        <div className="w-1.5 h-1.5 bg-white rounded-full" />
                        ACQUIRING DATA...
                      </div>
                    )}
                  </Card>

                  <div className="grid grid-cols-4 gap-4">
                    <ValueCard label="Yield Stress" value={isRunning ? (liveMetrics.stress * 0.85).toFixed(1) : (activeSample.status === "Completed" ? activeSample.result.split(' ')[0] : "0.0")} unit="MPa" />
                    <ValueCard label="Peak Load" value={isRunning ? (liveMetrics.force).toFixed(2) : "0.00"} unit="N" />
                    <ValueCard label="Young's Modulus" value="1.42" unit="GPa" trend="+0.05" />
                    <ValueCard label="Energy to Break" value="12.4" unit="J" color="text-emerald-400" />
                  </div>
                </div>
              </div>

              <div className="w-64 bg-[#1e293b] border-l border-slate-700 flex flex-col">
                <div className="p-4 border-b border-slate-700">
                  <h2 className="text-[10px] font-black uppercase tracking-tighter text-slate-400 mb-4">Manual Controls</h2>
                  <div className="grid grid-cols-2 gap-2">
                    <HardwareBtn icon={<ChevronDown size={16} />} label="Jog Down" onClick={() => handleJog('down')} />
                    <HardwareBtn icon={<ChevronDown className="rotate-180" size={16} />} label="Jog Up" onClick={() => handleJog('up')} />
                    <HardwareBtn icon={<Monitor size={16} />} label="Zero Load" onClick={() => { setLiveMetrics(m => ({...m, force: 0, stress: 0})); toast.success("Load Cell Zeroed"); }} />
                    <HardwareBtn icon={<Monitor size={16} />} label="Zero Ext" onClick={() => { setLiveMetrics(m => ({...m, extension: 0, strain: 0})); toast.success("Extension Zeroed"); }} />
                  </div>
                </div>
                
                <div className="p-4 flex-1 space-y-4">
                  <h2 className="text-[10px] font-black uppercase tracking-tighter text-slate-400">Internal Sequence</h2>
                  <div className="space-y-3">
                    <StatusStep label="Grip Engagement" status={sequenceStatus.grip} />
                    <StatusStep label="Slack Removal" status={sequenceStatus.slack} />
                    <StatusStep label="Main Test Path" status={sequenceStatus.main} active={isRunning} />
                    <StatusStep label="Data Buffering" status={sequenceStatus.buffer} active={isRunning} />
                    <StatusStep label="Peak Detection" status={sequenceStatus.peak} />
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
                <div>
                  <h2 className="text-3xl font-black text-white tracking-tighter">BATCH ANALYSIS</h2>
                  <p className="text-slate-500 font-medium">LIMS Integrated Database Results</p>
                </div>
                <div className="grid grid-cols-4 gap-4">
                  <StatBox label="Tests Run" value={samples.filter(s => s.status === "Completed").length.toString()} />
                  <StatBox label="Pass Rate" value="98.2%" color="text-green-400" />
                  <StatBox label="Active Samples" value={samples.length.toString()} />
                  <StatBox label="Machine Time" value="14.2h" />
                </div>
                <Card className="bg-[#1e293b] border-slate-700 overflow-hidden">
                  <table className="w-full text-left">
                    <thead className="bg-slate-900/50 text-[10px] font-black uppercase text-slate-500 border-b border-slate-700">
                      <tr>
                        <th className="px-6 py-4">Sample ID</th>
                        <th className="px-6 py-4">Strength</th>
                        <th className="px-6 py-4">Status</th>
                        <th className="px-6 py-4 text-right">Actions</th>
                      </tr>
                    </thead>
                    <tbody className="divide-y divide-slate-800">
                      {samples.map(sample => (
                        <tr key={sample.id} className="hover:bg-blue-600/5 transition-colors">
                          <td className="px-6 py-4 text-xs font-bold text-slate-300">{sample.name}</td>
                          <td className="px-6 py-4 text-xs font-mono text-blue-400">{sample.result}</td>
                          <td className="px-6 py-4">
                             <span className={`text-[8px] px-1.5 py-0.5 rounded-full font-black uppercase ${
                              sample.status === 'Completed' ? 'bg-green-500/20 text-green-400' : 'bg-slate-700 text-slate-400'
                            }`}>{sample.status}</span>
                          </td>
                          <td className="px-6 py-4 text-right">
                             <Button variant="ghost" size="sm" className="h-7 text-[10px] font-black text-blue-500" onClick={handleExport}>RECALL</Button>
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
                          <option>Offline</option>
                          <option>Maintenance</option>
                        </select>
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

function BigMeter({ label, value, unit, color }: { label: string; value: string; unit: string; color: string }) {
  return (
    <div className="flex flex-col justify-center px-8 border-r border-slate-800 transition-colors">
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

function StatusStep({ label, status, active }: { label: string; status: string; active?: boolean }) {
  const getStatusColor = () => {
    if (status === "Done") return "bg-green-500 border-green-400";
    if (status === "Active" || active) return "bg-blue-500 border-blue-400 animate-pulse ring-2 ring-blue-500/20";
    return "bg-slate-800 border-slate-700";
  };

  const getTextColor = () => {
    if (status === "Done") return "text-slate-400";
    if (status === "Active" || active) return "text-blue-400";
    return "text-slate-600";
  };

  return (
    <div className="flex items-center gap-3 group">
      <div className={`w-2 h-2 rounded-full border ${getStatusColor()}`} />
      <div className="flex flex-col">
        <span className={`text-[10px] font-bold uppercase tracking-tight ${getTextColor()}`}>{label}</span>
        <span className="text-[7px] font-black text-slate-500">{status}</span>
      </div>
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
