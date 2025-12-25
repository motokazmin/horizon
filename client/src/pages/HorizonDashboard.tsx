import { useState, useEffect } from "react";
import { Card, CardContent } from "@/components/ui/card";
import { Button } from "@/components/ui/button";
import { ScrollArea } from "@/components/ui/scroll-area";
import { 
  Play, 
  Square, 
  Settings, 
  FileText, 
  Activity, 
  Database, 
  TrendingUp,
  ChevronRight,
  Menu,
  Monitor,
  HardDrive
} from "lucide-react";
import {
  LineChart,
  Line,
  XAxis,
  YAxis,
  CartesianGrid,
  Tooltip,
  ResponsiveContainer,
} from "recharts";

const mockData = Array.from({ length: 50 }, (_, i) => ({
  time: i,
  force: Math.sin(i * 0.2) * 10 + i * 2 + Math.random() * 2,
  extension: i * 0.5,
}));

export default function HorizonDashboard() {
  const [activeTab, setActiveTab] = useState("test");
  const [isRunning, setIsRunning] = useState(false);
  const [liveValue, setLiveValue] = useState(0);

  useEffect(() => {
    let interval: NodeJS.Timeout;
    if (isRunning) {
      interval = setInterval(() => {
        setLiveValue(prev => prev + Math.random() * 5);
      }, 100);
    }
    return () => clearInterval(interval);
  }, [isRunning]);

  return (
    <div className="flex h-screen w-full bg-[#f0f2f5] text-slate-900 font-sans overflow-hidden">
      {/* Sidebar */}
      <aside className="w-16 bg-[#1a2b3c] flex flex-col items-center py-4 gap-6 shadow-xl z-20">
        <div className="w-10 h-10 bg-blue-500 rounded flex items-center justify-center text-white font-bold text-xl mb-4">
          T
        </div>
        <NavItem icon={<Activity size={24} />} active={activeTab === "test"} onClick={() => setActiveTab("test")} label="Test" />
        <NavItem icon={<Database size={24} />} active={activeTab === "data"} onClick={() => setActiveTab("data")} label="Data" />
        <NavItem icon={<FileText size={24} />} active={activeTab === "reports"} onClick={() => setActiveTab("reports")} label="Reports" />
        <NavItem icon={<Settings size={24} />} active={activeTab === "settings"} onClick={() => setActiveTab("settings")} label="Settings" />
        <div className="mt-auto">
          <NavItem icon={<Menu size={24} />} active={false} onClick={() => {}} label="Menu" />
        </div>
      </aside>

      {/* Main Content */}
      <main className="flex-1 flex flex-col relative overflow-hidden">
        {/* Header */}
        <header className="h-14 bg-white border-b flex items-center justify-between px-6 shadow-sm">
          <div className="flex items-center gap-4">
            <h1 className="text-lg font-bold text-[#1a2b3c] flex items-center gap-2">
              Horizon <span className="font-normal text-slate-400">|</span> 
              <span className="text-blue-600 uppercase text-sm tracking-widest">Tensile Test - ISO 527</span>
            </h1>
          </div>
          <div className="flex items-center gap-3">
            <div className="flex items-center gap-2 px-3 py-1 bg-green-50 text-green-700 rounded-full text-xs font-medium border border-green-100">
              <div className="w-2 h-2 rounded-full bg-green-500 animate-pulse" />
              Machine Connected
            </div>
            <Button variant="outline" size="sm" className="h-8 gap-2">
              <Monitor size={14} /> System Status
            </Button>
            <Button variant="outline" size="sm" className="h-8 gap-2">
              <HardDrive size={14} /> Database
            </Button>
          </div>
        </header>

        {/* Dashboard Area */}
        <div className="flex-1 p-6 flex gap-6 overflow-hidden">
          {/* Left: Controls & Stats */}
          <div className="w-80 flex flex-col gap-6">
            <Card className="border-none shadow-md overflow-hidden bg-[#1a2b3c] text-white">
              <CardContent className="p-0">
                <div className="bg-blue-600 px-4 py-2 text-xs font-bold uppercase tracking-widest">Live Feed</div>
                <div className="p-6 space-y-6">
                  <div>
                    <div className="text-blue-300 text-xs font-semibold uppercase mb-1">Force (N)</div>
                    <div className="text-5xl font-mono font-bold">{liveValue.toFixed(2)}</div>
                  </div>
                  <div className="grid grid-cols-2 gap-4 border-t border-slate-700 pt-6">
                    <div>
                      <div className="text-slate-400 text-[10px] uppercase font-bold mb-1">Extension</div>
                      <div className="text-xl font-mono">{(liveValue * 0.05).toFixed(3)} mm</div>
                    </div>
                    <div>
                      <div className="text-slate-400 text-[10px] uppercase font-bold mb-1">Stress</div>
                      <div className="text-xl font-mono">{(liveValue / 10).toFixed(2)} MPa</div>
                    </div>
                  </div>
                </div>
                <div className="p-4 bg-slate-800 flex gap-2">
                  <Button 
                    className={`flex-1 ${isRunning ? 'bg-red-500 hover:bg-red-600' : 'bg-green-500 hover:bg-green-600'} text-white border-none h-12 gap-2 text-lg font-bold transition-all`}
                    onClick={() => setIsRunning(!isRunning)}
                    data-testid="button-test-toggle"
                  >
                    {isRunning ? <Square size={20} fill="currentColor" /> : <Play size={20} fill="currentColor" />}
                    {isRunning ? "STOP" : "START"}
                  </Button>
                </div>
              </CardContent>
            </Card>

            <Card className="flex-1 border-none shadow-md overflow-hidden">
              <CardContent className="p-0 flex flex-col h-full">
                <div className="bg-slate-100 px-4 py-2 border-b flex justify-between items-center">
                  <span className="text-xs font-bold uppercase text-slate-500 tracking-wider">Test Sequence</span>
                  <TrendingUp size={14} className="text-blue-500" />
                </div>
                <ScrollArea className="flex-1">
                  <div className="p-4 space-y-4">
                    <SequenceItem label="Tension" value="Active" status="running" />
                    <SequenceItem label="Pre-load" value="Completed" status="done" />
                    <SequenceItem label="Grip Check" value="Completed" status="done" />
                    <SequenceItem label="Yield Point" value="Waiting" status="pending" />
                    <SequenceItem label="Break Point" value="Waiting" status="pending" />
                  </div>
                </ScrollArea>
              </CardContent>
            </Card>
          </div>

          {/* Right: Visualization */}
          <div className="flex-1 flex flex-col gap-6">
            <Card className="flex-1 border-none shadow-md overflow-hidden bg-white">
              <CardContent className="p-0 flex flex-col h-full">
                <div className="p-4 border-b flex justify-between items-center bg-slate-50">
                  <div className="flex gap-4">
                    <button className="text-sm font-bold border-b-2 border-blue-500 pb-1">Stress vs Strain</button>
                    <button className="text-sm font-medium text-slate-400 pb-1 hover:text-slate-600">Force vs Time</button>
                    <button className="text-sm font-medium text-slate-400 pb-1 hover:text-slate-600">Extension vs Load</button>
                  </div>
                  <div className="flex gap-2">
                    <Button variant="ghost" size="icon" className="h-8 w-8 text-slate-400 hover:text-blue-500">
                      <Settings size={16} />
                    </Button>
                  </div>
                </div>
                <div className="flex-1 p-6">
                  <ResponsiveContainer width="100%" height="100%">
                    <LineChart data={mockData}>
                      <CartesianGrid strokeDasharray="3 3" vertical={false} stroke="#f0f0f0" />
                      <XAxis 
                        dataKey="extension" 
                        label={{ value: 'Strain (%)', position: 'bottom', offset: 0, fontSize: 12, fill: '#94a3b8' }}
                        tick={{ fontSize: 10, fill: '#94a3b8' }}
                      />
                      <YAxis 
                        label={{ value: 'Stress (MPa)', angle: -90, position: 'left', fontSize: 12, fill: '#94a3b8' }}
                        tick={{ fontSize: 10, fill: '#94a3b8' }}
                      />
                      <Tooltip 
                        contentStyle={{ backgroundColor: '#1a2b3c', color: '#fff', border: 'none', borderRadius: '8px', fontSize: '12px' }}
                        itemStyle={{ color: '#60a5fa' }}
                      />
                      <Line 
                        type="monotone" 
                        dataKey="force" 
                        stroke="#2563eb" 
                        strokeWidth={3} 
                        dot={false}
                        animationDuration={1500}
                      />
                    </LineChart>
                  </ResponsiveContainer>
                </div>
              </CardContent>
            </Card>

            <Card className="h-48 border-none shadow-md overflow-hidden bg-white">
              <CardContent className="p-0 flex flex-col h-full">
                <div className="px-4 py-2 border-b bg-slate-50 flex justify-between items-center">
                  <span className="text-xs font-bold uppercase text-slate-500 tracking-wider">Results Matrix</span>
                  <div className="flex gap-4 text-[10px] font-bold text-slate-400">
                    <span className="flex items-center gap-1"><div className="w-2 h-2 rounded-full bg-blue-500" /> Current</span>
                    <span className="flex items-center gap-1"><div className="w-2 h-2 rounded-full bg-slate-200" /> Reference</span>
                  </div>
                </div>
                <div className="flex-1 grid grid-cols-4 divide-x divide-slate-100">
                  <ResultStat label="Peak Load" value="45.2" unit="kN" />
                  <ResultStat label="Modulus" value="2.8" unit="GPa" />
                  <ResultStat label="Elongation" value="12.4" unit="%" />
                  <ResultStat label="Yield Stress" value="38.5" unit="MPa" />
                </div>
              </CardContent>
            </Card>
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
      title={label}
      data-testid={`nav-item-${label.toLowerCase()}`}
    >
      {icon}
      {!active && (
        <div className="absolute left-14 bg-[#1a2b3c] text-white text-[10px] px-2 py-1 rounded opacity-0 group-hover:opacity-100 transition-opacity whitespace-nowrap pointer-events-none uppercase tracking-widest font-bold border border-slate-700">
          {label}
        </div>
      )}
    </button>
  );
}

function SequenceItem({ label, value, status }: { label: string; value: string; status: "running" | "done" | "pending" }) {
  const statusColors = {
    running: "bg-blue-500",
    done: "bg-green-500",
    pending: "bg-slate-200"
  };

  return (
    <div className="flex items-center justify-between p-3 rounded-lg bg-slate-50 border border-slate-100 group hover:border-blue-200 transition-colors">
      <div className="flex items-center gap-3">
        <div className={`w-2 h-2 rounded-full ${statusColors[status]} ${status === 'running' ? 'animate-pulse' : ''}`} />
        <span className="text-sm font-semibold text-slate-700">{label}</span>
      </div>
      <span className={`text-[10px] font-bold uppercase px-2 py-1 rounded ${
        status === 'done' ? 'text-green-600 bg-green-50' : 
        status === 'running' ? 'text-blue-600 bg-blue-50' : 
        'text-slate-400 bg-slate-100'
      }`}>
        {value}
      </span>
    </div>
  );
}

function ResultStat({ label, value, unit }: { label: string; value: string; unit: string }) {
  return (
    <div className="p-6 flex flex-col justify-center items-center text-center group hover:bg-slate-50 transition-colors">
      <span className="text-[10px] uppercase font-bold text-slate-400 mb-2 tracking-widest">{label}</span>
      <div className="flex items-baseline gap-1">
        <span className="text-3xl font-bold text-[#1a2b3c] tracking-tighter">{value}</span>
        <span className="text-xs font-bold text-slate-400">{unit}</span>
      </div>
      <div className="mt-2 text-[10px] text-green-500 font-bold">+2.4% vs avg</div>
    </div>
  );
}
