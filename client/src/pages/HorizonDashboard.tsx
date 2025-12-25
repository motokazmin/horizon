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
  Legend
} from "recharts";
import { toast } from "sonner";

const mockData = Array.from({ length: 50 }, (_, i) => ({
  time: i,
  force: Math.sin(i * 0.2) * 10 + i * 2 + Math.random() * 2,
  extension: i * 0.5,
  stress: (Math.sin(i * 0.2) * 10 + i * 2) / 10,
}));

export default function HorizonDashboard() {
  const [activeTab, setActiveTab] = useState("test");
  const [isRunning, setIsRunning] = useState(false);
  const [liveValue, setLiveValue] = useState(0);
  const [samples, setSamples] = useState([
    { id: 1, name: "Sample A-101", status: "Ready", result: "-" },
    { id: 2, name: "Sample A-102", status: "Ready", result: "-" },
  ]);
  const [activeSample, setActiveSample] = useState(1);

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
              <span className="text-blue-600 uppercase text-sm tracking-widest">Method: Tensile ISO 527-2</span>
            </h1>
          </div>
          <div className="flex items-center gap-3">
            <div className="flex items-center gap-4 mr-4 text-xs font-semibold text-slate-500 border-r pr-4">
              <span className="flex items-center gap-1"><div className="w-2 h-2 rounded-full bg-blue-500" /> STDM-100kN</span>
              <span className="flex items-center gap-1 text-green-600"><div className="w-2 h-2 rounded-full bg-green-500" /> ONLINE</span>
            </div>
            <Button variant="ghost" size="sm" className="h-8 gap-2"><Save size={14} /> Save Batch</Button>
            <Button variant="ghost" size="sm" className="h-8 gap-2"><Download size={14} /> Export</Button>
          </div>
        </header>

        <div className="flex-1 flex overflow-hidden">
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
                <div className="space-y-1.5 pt-2">
                  <Button variant="outline" className="w-full h-8 text-xs gap-2"><Copy size={12} /> Clone Config</Button>
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
                <div className="flex flex-col items-end mr-4">
                  <span className="text-[10px] font-bold text-slate-500 uppercase">Load Cell</span>
                  <span className="text-xs font-bold text-white tracking-wider">100 kN Range</span>
                </div>
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
              {/* Main Chart Card */}
              <Card className="border-none shadow-md overflow-hidden bg-white flex flex-col">
                <div className="px-4 py-3 border-b flex justify-between items-center bg-white">
                  <Tabs defaultValue="stress-strain" className="w-[400px]">
                    <TabsList className="bg-slate-100 h-8 p-1">
                      <TabsTrigger value="stress-strain" className="text-[10px] uppercase font-bold h-6">Stress vs Strain</TabsTrigger>
                      <TabsTrigger value="force-time" className="text-[10px] uppercase font-bold h-6">Force vs Time</TabsTrigger>
                    </TabsList>
                  </Tabs>
                  <div className="flex items-center gap-2">
                    <Button variant="ghost" size="icon" className="h-8 w-8 text-slate-400"><AlertTriangle size={16} /></Button>
                    <Button variant="ghost" size="icon" className="h-8 w-8 text-slate-400"><Settings size={16} /></Button>
                  </div>
                </div>
                <div className="flex-1 p-4">
                  <ResponsiveContainer width="100%" height="100%">
                    <LineChart data={mockData}>
                      <CartesianGrid strokeDasharray="3 3" vertical={false} stroke="#f1f5f9" />
                      <XAxis dataKey="extension" tick={{fontSize: 10}} />
                      <YAxis tick={{fontSize: 10}} />
                      <Tooltip 
                        contentStyle={{ backgroundColor: '#1a2b3c', color: '#fff', border: 'none', borderRadius: '4px' }}
                      />
                      <Legend verticalAlign="top" align="right" />
                      <Line name="Current" type="monotone" dataKey="stress" stroke="#2563eb" strokeWidth={2} dot={false} />
                      <Line name="Previous" type="monotone" dataKey="stress" stroke="#e2e8f0" strokeWidth={2} dot={false} strokeDasharray="5 5" />
                    </LineChart>
                  </ResponsiveContainer>
                </div>
              </Card>

              {/* Bottom Result Matrix */}
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

          {/* Right Sidebar: Machine Status & Controls */}
          <div className="w-64 bg-white border-l flex flex-col">
            <div className="p-4 border-b">
              <h2 className="text-xs font-bold uppercase tracking-wider text-slate-500 mb-4">Machine Controls</h2>
              <div className="grid grid-cols-2 gap-2">
                <ControlButton icon={<ChevronDown size={14} />} label="Jog Down" />
                <ControlButton icon={<ChevronDown className="rotate-180" size={14} />} label="Jog Up" />
                <ControlButton icon={<Monitor size={14} />} label="Zero Force" />
                <ControlButton icon={<Monitor size={14} />} label="Zero Ext" />
              </div>
            </div>
            
            <div className="p-4 flex-1">
              <h2 className="text-xs font-bold uppercase tracking-wider text-slate-500 mb-4">Sequence Timeline</h2>
              <div className="relative pl-4 space-y-6 before:absolute before:left-[4px] before:top-2 before:bottom-2 before:w-[2px] before:bg-slate-100">
                <TimelineItem label="Approach" status="Done" time="0.0s" active={false} />
                <TimelineItem label="Pre-load (50N)" status="Done" time="2.4s" active={false} />
                <TimelineItem label="Primary Test" status={isRunning ? "Active" : "Ready"} time="--" active={isRunning} />
                <TimelineItem label="Yield Detection" status="Pending" time="--" active={false} />
                <TimelineItem label="Fracture Analysis" status="Pending" time="--" active={false} />
              </div>
            </div>

            <div className="p-4 bg-red-50 mt-auto border-t">
              <Button variant="destructive" className="w-full h-12 uppercase font-black tracking-widest text-sm flex-col gap-0 leading-tight">
                <span>EMERGENCY</span>
                <span className="text-[10px] opacity-80">STOP</span>
              </Button>
            </div>
          </div>
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

function ControlButton({ icon, label }: { icon: React.ReactNode; label: string }) {
  return (
    <Button variant="outline" className="flex flex-col h-14 gap-1 p-2 bg-slate-50 hover:bg-blue-50 hover:border-blue-200">
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
