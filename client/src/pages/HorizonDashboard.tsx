import { useState, useEffect } from "react";
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
  BarChart3
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
  Area
} from "recharts";
import { toast } from "sonner";

const mockData = Array.from({ length: 50 }, (_, i) => ({
  time: i,
  force: Math.sin(i * 0.2) * 10 + i * 2 + Math.random() * 2,
  extension: i * 0.5,
  stress: (Math.sin(i * 0.2) * 10 + i * 2) / 10,
}));

const historicalData = [
  { batch: "B-202", avgStress: 45.2, date: "2025-12-20" },
  { batch: "B-203", avgStress: 48.7, date: "2025-12-21" },
  { batch: "B-204", avgStress: 42.1, date: "2025-12-22" },
  { batch: "B-205", avgStress: 50.3, date: "2025-12-23" },
];

export default function HorizonDashboard() {
  const [activeTab, setActiveTab] = useState("test");
  const [isRunning, setIsRunning] = useState(false);
  const [liveValue, setLiveValue] = useState(0);
  const [samples, setSamples] = useState([
    { id: 1, name: "Sample A-101", status: "Ready", result: "-" },
    { id: 2, name: "Sample A-102", status: "Ready", result: "-" },
  ]);
  const [activeSample, setActiveSample] = useState(1);

  // States for Machine Config
  const [machineStatus, setMachineStatus] = useState("Online");
  const [loadCell, setLoadCell] = useState("STDM-100kN");

  useEffect(() => {
    let interval: NodeJS.Timeout;
    if (isRunning) {
      interval = setInterval(() => {
        setLiveValue(prev => prev + Math.random() * 5);
      }, 100);
    }
    return () => clearInterval(interval);
  }, [isRunning]);

  const handleStart = () => {
    setIsRunning(true);
    toast.success("Test sequence initiated");
  };

  const handleStop = () => {
    setIsRunning(false);
    toast.error("Test sequence halted by user");
    setSamples(prev => prev.map(s => 
      s.id === activeSample ? { ...s, status: "Completed", result: (liveValue/10).toFixed(2) + " MPa" } : s
    ));
  };

  const addSample = () => {
    const newId = samples.length + 1;
    setSamples([...samples, { id: newId, name: `Sample A-${100 + newId}`, status: "Ready", result: "-" }]);
  };

  return (
    <div className="flex h-screen w-full bg-[#f0f2f5] text-slate-900 font-sans overflow-hidden">
      {/* Sidebar */}
      <aside className="w-16 bg-[#1a2b3c] flex flex-col items-center py-4 gap-6 shadow-xl z-20">
        <div className="w-10 h-10 bg-blue-500 rounded flex items-center justify-center text-white font-bold text-xl mb-4">
          T
        </div>
        <NavItem icon={<Activity size={24} />} active={activeTab === "test"} onClick={() => setActiveTab("test")} label="Test" />
        <NavItem icon={<History size={24} />} active={activeTab === "data"} onClick={() => setActiveTab("data")} label="Batch Data" />
        <NavItem icon={<FileText size={24} />} active={activeTab === "reports"} onClick={() => setActiveTab("reports")} label="Reports" />
        <NavItem icon={<Settings size={24} />} active={activeTab === "settings"} onClick={() => setActiveTab("settings")} label="Machine Config" />
        <div className="mt-auto flex flex-col gap-4">
          <NavItem icon={<Info size={24} />} active={false} onClick={() => {}} label="Help" />
        </div>
      </aside>

      {/* Main Content */}
      <main className="flex-1 flex flex-col relative overflow-hidden">
        {/* Header */}
        <header className="h-14 bg-white border-b flex items-center justify-between px-6 shadow-sm z-10">
          <div className="flex items-center gap-4">
            <h1 className="text-lg font-bold text-[#1a2b3c] flex items-center gap-2">
              Horizon <span className="font-normal text-slate-400">|</span> 
              <span className="text-blue-600 uppercase text-sm tracking-widest">
                {activeTab === "test" && "Method: Tensile ISO 527-2"}
                {activeTab === "data" && "Batch History & Analytics"}
                {activeTab === "reports" && "Report Generator"}
                {activeTab === "settings" && "Machine Configuration"}
              </span>
            </h1>
          </div>
          <div className="flex items-center gap-3">
            <div className="flex items-center gap-4 mr-4 text-xs font-semibold text-slate-500 border-r pr-4">
              <span className="flex items-center gap-1"><div className="w-2 h-2 rounded-full bg-blue-500" /> {loadCell}</span>
              <span className={`flex items-center gap-1 ${machineStatus === 'Online' ? 'text-green-600' : 'text-red-600'}`}>
                <div className={`w-2 h-2 rounded-full ${machineStatus === 'Online' ? 'bg-green-500' : 'bg-red-500'}`} /> {machineStatus.toUpperCase()}
              </span>
            </div>
            <Button variant="ghost" size="sm" className="h-8 gap-2"><Save size={14} /> Save Batch</Button>
            <Button variant="ghost" size="sm" className="h-8 gap-2"><Download size={14} /> Export</Button>
          </div>
        </header>

        <div className="flex-1 flex overflow-hidden">
          {activeTab === "test" && (
            <>
              {/* Left Panel: Batch Management */}
              <div className="w-72 bg-white border-r flex flex-col">
                <div className="p-4 border-b bg-slate-50">
                  <div className="flex items-center justify-between mb-4">
                    <h2 className="text-xs font-bold uppercase tracking-wider text-slate-500">Batch Samples</h2>
                    <Button variant="outline" size="icon" className="h-7 w-7" onClick={addSample}><Plus size={14} /></Button>
                  </div>
                  <div className="space-y-1">
                    {samples.map(sample => (
                      <div 
                        key={sample.id}
                        onClick={() => setActiveSample(sample.id)}
                        className={`p-3 rounded-lg border cursor-pointer transition-all ${
                          activeSample === sample.id 
                            ? 'bg-blue-50 border-blue-200 ring-1 ring-blue-100' 
                            : 'bg-white border-slate-100 hover:border-slate-300'
                        }`}
                      >
                        <div className="flex justify-between items-start mb-1">
                          <span className={`text-xs font-bold ${activeSample === sample.id ? 'text-blue-700' : 'text-slate-700'}`}>{sample.name}</span>
                          <span className={`text-[9px] px-1.5 py-0.5 rounded-full font-bold uppercase ${
                            sample.status === 'Completed' ? 'bg-green-100 text-green-700' : 'bg-slate-100 text-slate-500'
                          }`}>{sample.status}</span>
                        </div>
                        <div className="text-[10px] text-slate-400 flex justify-between">
                          <span>Res: {sample.result}</span>
                          {activeSample === sample.id && <TrendingUp size={10} className="text-blue-500" />}
                        </div>
                      </div>
                    ))}
                  </div>
                </div>
                
                <div className="p-4 flex-1 overflow-auto">
                  <h2 className="text-xs font-bold uppercase tracking-wider text-slate-500 mb-4">Sample Parameters</h2>
                  <div className="space-y-4">
                    <div className="space-y-1.5">
                      <Label className="text-[10px] uppercase font-bold text-slate-400">Width (mm)</Label>
                      <Input defaultValue="10.00" className="h-8 text-sm font-mono" />
                    </div>
                    <div className="space-y-1.5">
                      <Label className="text-[10px] uppercase font-bold text-slate-400">Thickness (mm)</Label>
                      <Input defaultValue="4.00" className="h-8 text-sm font-mono" />
                    </div>
                    <div className="space-y-1.5">
                      <Label className="text-[10px] uppercase font-bold text-slate-400">Gauge Length (mm)</Label>
                      <Input defaultValue="50.00" className="h-8 text-sm font-mono" />
                    </div>
                  </div>
                </div>
              </div>

              {/* Center: Live Data & Charts */}
              <div className="flex-1 flex flex-col bg-[#f8fafc]">
                {/* Live Meter Strip */}
                <div className="bg-[#1a2b3c] p-4 flex gap-8 shadow-inner overflow-x-auto">
                  <LiveValue label="Force" value={liveValue.toFixed(2)} unit="N" color="text-blue-400" />
                  <LiveValue label="Extension" value={(liveValue * 0.05).toFixed(3)} unit="mm" color="text-emerald-400" />
                  <LiveValue label="Stress" value={(liveValue / 10).toFixed(2)} unit="MPa" color="text-amber-400" />
                  <LiveValue label="Strain" value={(liveValue * 0.1).toFixed(2)} unit="%" color="text-purple-400" />
                  
                  <div className="ml-auto flex items-center gap-3">
                    <Button 
                      onClick={isRunning ? handleStop : handleStart}
                      className={`w-40 h-12 text-lg font-bold gap-2 shadow-lg transition-all active:scale-95 ${
                        isRunning ? 'bg-red-600 hover:bg-red-700' : 'bg-green-600 hover:bg-green-700'
                      }`}
                    >
                      {isRunning ? <Square size={20} fill="currentColor" /> : <Play size={20} fill="currentColor" />}
                      {isRunning ? "STOP TEST" : "RUN TEST"}
                    </Button>
                  </div>
                </div>

                <div className="flex-1 p-6 grid grid-rows-[1fr_200px] gap-6 overflow-hidden">
                  <Card className="border-none shadow-md overflow-hidden bg-white flex flex-col">
                    <div className="px-4 py-3 border-b flex justify-between items-center bg-white">
                      <Tabs defaultValue="stress-strain" className="w-[400px]">
                        <TabsList className="bg-slate-100 h-8 p-1">
                          <TabsTrigger value="stress-strain" className="text-[10px] uppercase font-bold h-6">Stress vs Strain</TabsTrigger>
                          <TabsTrigger value="force-time" className="text-[10px] uppercase font-bold h-6">Force vs Time</TabsTrigger>
                        </TabsList>
                      </Tabs>
                    </div>
                    <div className="flex-1 p-4">
                      <ResponsiveContainer width="100%" height="100%">
                        <LineChart data={mockData}>
                          <CartesianGrid strokeDasharray="3 3" vertical={false} stroke="#f1f5f9" />
                          <XAxis dataKey="extension" tick={{fontSize: 10}} />
                          <YAxis tick={{fontSize: 10}} />
                          <Tooltip contentStyle={{ backgroundColor: '#1a2b3c', color: '#fff', border: 'none', borderRadius: '4px' }} />
                          <Line name="Current" type="monotone" dataKey="stress" stroke="#2563eb" strokeWidth={2} dot={false} />
                        </LineChart>
                      </ResponsiveContainer>
                    </div>
                  </Card>

                  <Card className="border-none shadow-md overflow-hidden bg-white">
                    <div className="h-full grid grid-cols-5 divide-x">
                      <ResultBox label="Peak Stress" value="48.2" unit="MPa" sub="Target: 50.0" />
                      <ResultBox label="Break Force" value="12.4" unit="kN" sub="Spec: >10.0" />
                      <ResultBox label="Young's Modulus" value="3.12" unit="GPa" sub="+5.2% Shift" />
                      <ResultBox label="Strain @ Break" value="14.8" unit="%" sub="Limit: 20%" />
                      <div className="p-4 bg-slate-50 flex flex-col justify-center gap-2">
                        <Button className="w-full bg-[#1a2b3c] h-8 text-xs font-bold uppercase tracking-wider">Accept Result</Button>
                        <Button variant="outline" className="w-full border-red-200 text-red-600 hover:bg-red-50 h-8 text-xs font-bold uppercase tracking-wider">Discard</Button>
                      </div>
                    </div>
                  </Card>
                </div>
              </div>

              {/* Right Sidebar */}
              <div className="w-64 bg-white border-l flex flex-col">
                <div className="p-4 border-b">
                  <h2 className="text-xs font-bold uppercase tracking-wider text-slate-500 mb-4">Machine Controls</h2>
                  <div className="grid grid-cols-2 gap-2">
                    <ControlButton icon={<ChevronDown size={14} />} label="Jog Down" onClick={() => toast("Jogging Down...")} />
                    <ControlButton icon={<ChevronDown className="rotate-180" size={14} />} label="Jog Up" onClick={() => toast("Jogging Up...")} />
                    <ControlButton icon={<Monitor size={14} />} label="Zero Force" onClick={() => { setLiveValue(0); toast.info("Force Zeroed"); }} />
                    <ControlButton icon={<Monitor size={14} />} label="Zero Ext" onClick={() => toast.info("Extension Zeroed")} />
                  </div>
                </div>
                <div className="p-4 flex-1">
                  <h2 className="text-xs font-bold uppercase tracking-wider text-slate-500 mb-4">Sequence</h2>
                  <div className="relative pl-4 space-y-6 before:absolute before:left-[4px] before:top-2 before:bottom-2 before:w-[2px] before:bg-slate-100">
                    <TimelineItem label="Approach" status="Done" time="0.0s" active={false} />
                    <TimelineItem label="Primary Test" status={isRunning ? "Active" : "Ready"} time="--" active={isRunning} />
                  </div>
                </div>
                <div className="p-4 bg-red-50 mt-auto border-t">
                  <Button variant="destructive" className="w-full h-12 uppercase font-black tracking-widest text-sm flex-col gap-0 leading-tight" onClick={handleStop}>
                    <span>EMERGENCY</span>
                    <span className="text-[10px] opacity-80">STOP</span>
                  </Button>
                </div>
              </div>
            </>
          )}

          {activeTab === "data" && (
            <div className="flex-1 p-8 bg-white overflow-auto">
              <div className="max-w-5xl mx-auto space-y-8">
                <div className="flex justify-between items-end">
                  <div>
                    <h2 className="text-2xl font-bold text-slate-900">Batch History</h2>
                    <p className="text-slate-500">View and analyze previous test results</p>
                  </div>
                  <div className="flex gap-2">
                    <Button variant="outline" className="gap-2"><Search size={14} /> Search Records</Button>
                  </div>
                </div>

                <div className="grid grid-cols-3 gap-6">
                  <Card className="p-6 bg-blue-50 border-blue-100">
                    <h3 className="text-xs font-bold uppercase text-blue-600 mb-2">Total Tests</h3>
                    <div className="text-3xl font-bold text-blue-900">1,248</div>
                  </Card>
                  <Card className="p-6 bg-green-50 border-green-100">
                    <h3 className="text-xs font-bold uppercase text-green-600 mb-2">Pass Rate</h3>
                    <div className="text-3xl font-bold text-green-900">98.2%</div>
                  </Card>
                  <Card className="p-6 bg-amber-50 border-amber-100">
                    <h3 className="text-xs font-bold uppercase text-amber-600 mb-2">Avg. Stress</h3>
                    <div className="text-3xl font-bold text-amber-900">46.5 MPa</div>
                  </Card>
                </div>

                <Card className="overflow-hidden border-slate-200">
                  <table className="w-full text-left">
                    <thead className="bg-slate-50 border-b border-slate-200">
                      <tr>
                        <th className="px-6 py-3 text-xs font-bold text-slate-500 uppercase">Batch ID</th>
                        <th className="px-6 py-3 text-xs font-bold text-slate-500 uppercase">Avg Stress</th>
                        <th className="px-6 py-3 text-xs font-bold text-slate-500 uppercase">Date</th>
                        <th className="px-6 py-3 text-xs font-bold text-slate-500 uppercase text-right">Action</th>
                      </tr>
                    </thead>
                    <tbody className="divide-y divide-slate-100">
                      {historicalData.map(item => (
                        <tr key={item.batch} className="hover:bg-slate-50 transition-colors">
                          <td className="px-6 py-4 font-bold text-slate-700">{item.batch}</td>
                          <td className="px-6 py-4 font-mono">{item.avgStress} MPa</td>
                          <td className="px-6 py-4 text-slate-500">{item.date}</td>
                          <td className="px-6 py-4 text-right">
                            <Button variant="ghost" size="sm" className="text-blue-600">View Details</Button>
                          </td>
                        </tr>
                      ))}
                    </tbody>
                  </table>
                </Card>

                <Card className="p-6">
                  <h3 className="text-sm font-bold text-slate-800 mb-6">Historical Performance Trend</h3>
                  <div className="h-[300px]">
                    <ResponsiveContainer width="100%" height="100%">
                      <AreaChart data={historicalData}>
                        <CartesianGrid strokeDasharray="3 3" vertical={false} />
                        <XAxis dataKey="batch" />
                        <YAxis />
                        <Tooltip />
                        <Area type="monotone" dataKey="avgStress" stroke="#2563eb" fill="#dbeafe" />
                      </AreaChart>
                    </ResponsiveContainer>
                  </div>
                </Card>
              </div>
            </div>
          )}

          {activeTab === "reports" && (
            <div className="flex-1 p-8 bg-slate-50 flex items-center justify-center">
              <Card className="max-w-md w-full p-8 text-center space-y-6">
                <div className="w-16 h-16 bg-blue-100 text-blue-600 rounded-full flex items-center justify-center mx-auto">
                  <FileText size={32} />
                </div>
                <div>
                  <h2 className="text-xl font-bold">Report Generator</h2>
                  <p className="text-slate-500 mt-2">Select a batch to generate a PDF compliance report.</p>
                </div>
                <div className="space-y-3">
                  <Button className="w-full h-12 gap-2"><Plus size={16} /> Create New Report</Button>
                  <Button variant="outline" className="w-full h-12 gap-2"><History size={16} /> View Recent Reports</Button>
                </div>
              </Card>
            </div>
          )}

          {activeTab === "settings" && (
            <div className="flex-1 p-8 bg-white overflow-auto">
              <div className="max-w-3xl mx-auto space-y-12">
                <section>
                  <h2 className="text-xl font-bold mb-6">Machine Settings</h2>
                  <div className="grid grid-cols-2 gap-8">
                    <div className="space-y-4">
                      <div className="space-y-1.5">
                        <Label>Machine Status</Label>
                        <select 
                          className="w-full h-10 px-3 border rounded-md text-sm"
                          value={machineStatus}
                          onChange={(e) => setMachineStatus(e.target.value)}
                        >
                          <option>Online</option>
                          <option>Offline</option>
                          <option>Maintenance</option>
                        </select>
                      </div>
                      <div className="space-y-1.5">
                        <Label>Load Cell Configuration</Label>
                        <select 
                          className="w-full h-10 px-3 border rounded-md text-sm"
                          value={loadCell}
                          onChange={(e) => setLoadCell(e.target.value)}
                        >
                          <option>STDM-100kN</option>
                          <option>STDM-50kN</option>
                          <option>STDM-10kN</option>
                        </select>
                      </div>
                    </div>
                    <div className="space-y-4">
                      <div className="p-4 bg-slate-50 rounded-lg border border-dashed">
                        <h4 className="text-xs font-bold uppercase text-slate-500 mb-2">Calibration Info</h4>
                        <div className="text-xs space-y-1">
                          <p>Last Calibrated: 2025-06-15</p>
                          <p>Due Date: 2026-06-15</p>
                          <p className="text-green-600 font-bold">Status: Valid</p>
                        </div>
                      </div>
                    </div>
                  </div>
                </section>

                <section className="pt-8 border-t">
                  <h2 className="text-xl font-bold mb-6">User Permissions</h2>
                  <div className="space-y-4">
                    <div className="flex items-center justify-between p-4 border rounded-lg">
                      <div className="flex items-center gap-3">
                        <div className="w-10 h-10 bg-slate-100 rounded-full flex items-center justify-center"><Users size={20} /></div>
                        <div>
                          <p className="text-sm font-bold">Operator Mode</p>
                          <p className="text-xs text-slate-500">Run tests and view results only</p>
                        </div>
                      </div>
                      <Button variant="outline" size="sm">Switch</Button>
                    </div>
                    <div className="flex items-center justify-between p-4 border rounded-lg bg-blue-50 border-blue-100">
                      <div className="flex items-center gap-3">
                        <div className="w-10 h-10 bg-blue-100 text-blue-600 rounded-full flex items-center justify-center"><Settings size={20} /></div>
                        <div>
                          <p className="text-sm font-bold text-blue-900">Administrator Mode</p>
                          <p className="text-xs text-blue-700">Full access to methods and calibration</p>
                        </div>
                      </div>
                      <div className="text-[10px] font-bold text-blue-600 uppercase">Active</div>
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
      className={`w-10 h-10 rounded-lg flex items-center justify-center transition-all group relative ${
        active ? "bg-blue-600 text-white shadow-lg" : "text-slate-400 hover:bg-slate-800 hover:text-slate-200"
      }`}
      data-testid={`nav-item-${label.toLowerCase().replace(' ', '-')}`}
    >
      {icon}
      <div className="absolute left-14 bg-[#1a2b3c] text-white text-[10px] px-2 py-1 rounded opacity-0 group-hover:opacity-100 transition-opacity whitespace-nowrap pointer-events-none uppercase tracking-widest font-bold border border-slate-700 z-50 shadow-xl">
        {label}
      </div>
    </button>
  );
}

function LiveValue({ label, value, unit, color }: { label: string; value: string; unit: string; color: string }) {
  return (
    <div className="flex flex-col min-w-[120px]">
      <span className="text-[10px] font-bold text-slate-400 uppercase mb-1 tracking-wider">{label}</span>
      <div className="flex items-baseline gap-1">
        <span className={`text-2xl font-mono font-bold ${color}`}>{value}</span>
        <span className="text-[10px] font-bold text-slate-500 uppercase">{unit}</span>
      </div>
    </div>
  );
}

function ResultBox({ label, value, unit, sub }: { label: string; value: string; unit: string; sub: string }) {
  return (
    <div className="p-4 flex flex-col justify-center hover:bg-slate-50 transition-colors group">
      <span className="text-[9px] uppercase font-black text-slate-400 mb-1 tracking-widest group-hover:text-blue-500 transition-colors">{label}</span>
      <div className="flex items-baseline gap-1 mb-1">
        <span className="text-2xl font-bold text-slate-800 tracking-tight">{value}</span>
        <span className="text-[10px] font-bold text-slate-400 uppercase">{unit}</span>
      </div>
      <span className="text-[9px] font-bold text-slate-400 tracking-wide">{sub}</span>
    </div>
  );
}

function ControlButton({ icon, label, onClick }: { icon: React.ReactNode; label: string; onClick?: () => void }) {
  return (
    <Button variant="outline" className="flex flex-col h-14 gap-1 p-2 bg-slate-50 hover:bg-blue-50 hover:border-blue-200" onClick={onClick}>
      {icon}
      <span className="text-[9px] uppercase font-bold text-slate-500">{label}</span>
    </Button>
  );
}

function TimelineItem({ label, status, time, active }: { label: string; status: string; time: string; active: boolean }) {
  return (
    <div className="relative group">
      <div className={`absolute -left-[16px] top-1.5 w-2 h-2 rounded-full border-2 bg-white z-10 ${
        active ? 'border-blue-500 ring-2 ring-blue-100 scale-125' : 
        status === 'Done' ? 'border-green-500 bg-green-500' : 'border-slate-200'
      }`} />
      <div className="flex justify-between items-start">
        <div className="flex flex-col">
          <span className={`text-xs font-bold ${active ? 'text-blue-600' : 'text-slate-700'}`}>{label}</span>
          <span className="text-[9px] font-bold uppercase text-slate-400">{status}</span>
        </div>
        <span className="text-[9px] font-mono font-bold text-slate-400">{time}</span>
      </div>
    </div>
  );
}
