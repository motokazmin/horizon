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
  Download,
  Zap,
  Lock,
  Eye,
  Layers,
  Thermometer,
  Gauge,
  History,
  CheckCircle2,
  Clock,
  Shield,
  Cpu,
  Radio
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
import { motion, AnimatePresence } from "framer-motion";

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
    strain: 0,
    speed: 5.0,
    temp: 23.4
  });
  
  const [samples, setSamples] = useState([
    { id: 1, name: "Sample A-101", status: "Ready", result: "-", width: 10, thickness: 4, operator: "Admin", date: "2025-12-25" },
    { id: 2, name: "Sample A-102", status: "Ready", result: "-", width: 10, thickness: 4, operator: "Admin", date: "2025-12-25" },
  ]);
  const [activeSampleId, setActiveSampleId] = useState(1);
  const [machineStatus, setMachineStatus] = useState("Online");
  const [loadCell, setLoadCell] = useState("STDM-100kN");
  const [testMethod] = useState("Tensile ISO 527-2");
  
  // Demo State
  const [isDemoActive, setIsDemoActive] = useState(false);
  const [demoStep, setDemoStep] = useState(0);
  const [demoMessage, setDemoMessage] = useState("");

  // Visual Overlay State
  const [isSafetyDoorClosed, setIsSafetyDoorClosed] = useState(true);

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
          setLiveMetrics(prev => ({
            ...prev,
            force: parseFloat(point.force),
            extension: parseFloat(point.extension),
            stress: parseFloat(point.stress),
            strain: parseFloat(point.strain)
          }));
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

  // Demo Logic - FIXED
  const startDemo = async () => {
    if (isDemoActive || isRunning) return;
    
    setIsDemoActive(true);
    setActiveTab("test");
    
    const steps = [
      { msg: "Системный чек: Дверца безопасности закрыта...", action: () => { setIsSafetyDoorClosed(true); toast.success("Interlock Secure"); } },
      { msg: "Шаг 1: Подготовка образца и обнуление системы...", action: () => { setLiveMetrics(prev => ({ ...prev, force: 0, extension: 0, stress: 0, strain: 0 })); toast.info("Система обнулена"); } },
      { msg: "Шаг 2: Проверка захватов и удаление слабины...", action: () => { setSequenceStatus(s => ({ ...s, grip: "Done", slack: "Done" })); } },
      { msg: "Шаг 3: Запуск испытания по методу ISO 527-2...", action: () => { handleStart(); } },
      { msg: "Шаг 4: Сбор данных и построение кривой в реальном времени...", action: () => {} },
      { msg: "Шаг 5: Обнаружение пиковой нагрузки и завершение...", action: () => {} },
      { msg: "Шаг 6: Сохранение результатов в базу данных...", action: () => { toast.success("Данные успешно сохранены"); } },
      { msg: "Демонстрация завершена. Система готова к работе.", action: () => { setIsDemoActive(false); setDemoMessage(""); } }
    ];

    for (let i = 0; i < steps.length; i++) {
      setDemoStep(i);
      setDemoMessage(steps[i].msg);
      steps[i].action();
      
      if (i === 3) {
        await new Promise(r => setTimeout(r, 8500));
      } else {
        await new Promise(r => setTimeout(r, 3000));
      }
      
      // Check if demo was cancelled
      const isCancelled = await new Promise(resolve => {
        setIsDemoActive(current => {
          resolve(!current);
          return current;
        });
      });
      if (isCancelled) break;
    }
  };

  const handleStart = () => {
    if (machineStatus !== "Online") {
      toast.error("Machine must be ONLINE to start test");
      return;
    }
    if (!isSafetyDoorClosed) {
      toast.error("Safety door INTERLOCK triggered! Close door to proceed.");
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

  const handleExport = (sample?: any) => {
    const target = sample || activeSample;
    const reportData = {
      method: testMethod,
      sample: target.name,
      result: target.result,
      width: target.width,
      thickness: target.thickness,
      operator: target.operator,
      timestamp: new Date().toLocaleString()
    };
    
    const blob = new Blob([JSON.stringify(reportData, null, 2)], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `Horizon_Report_${target.name.replace(' ', '_')}.json`;
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
    setSamples([...samples, { id: newId, name: `Sample A-${100 + newId}`, status: "Ready", result: "-", width: 10, thickness: 4, operator: "Admin", date: new Date().toISOString().split('T')[0] }]);
  };

  const deleteSample = (id: number) => {
    if (samples.length > 1) {
      setSamples(samples.filter(s => s.id !== id));
      if (activeSampleId === id) setActiveSampleId(samples[0].id);
    }
  };

  return (
    <div className="flex h-screen w-full bg-[#0f172a] text-slate-200 font-sans overflow-hidden select-none relative">
      {/* Demo Overlay */}
      <AnimatePresence>
        {isDemoActive && demoMessage && (
          <motion.div 
            initial={{ opacity: 0, y: 50 }}
            animate={{ opacity: 1, y: 0 }}
            exit={{ opacity: 0, y: 50 }}
            className="absolute bottom-10 left-1/2 -translate-x-1/2 z-[100] bg-blue-600 px-6 py-4 rounded-xl shadow-2xl border border-blue-400 flex items-center gap-4 min-w-[500px]"
          >
            <div className="w-8 h-8 bg-white/20 rounded-full flex items-center justify-center animate-spin">
              <RefreshCw size={16} />
            </div>
            <div>
              <p className="text-[10px] font-black uppercase text-blue-200 tracking-widest mb-1">Demo Mode Active • Step {demoStep + 1}/8</p>
              <p className="text-sm font-bold text-white tracking-tight">{demoMessage}</p>
            </div>
            <Button 
              size="sm" 
              variant="ghost" 
              className="ml-auto text-white hover:bg-white/10"
              onClick={() => { setIsDemoActive(false); handleStop(false); }}
            >
              Skip Demo
            </Button>
          </motion.div>
        )}
      </AnimatePresence>

      <aside className="w-16 bg-[#020617] flex flex-col items-center py-4 gap-6 border-r border-slate-800 z-30">
        <div className="w-10 h-10 bg-blue-600 rounded flex items-center justify-center text-white font-black text-xl mb-4 shadow-lg shadow-blue-900/20">
          TO
        </div>
        <NavItem icon={<Activity size={22} />} active={activeTab === "test"} onClick={() => setActiveTab("test")} label="Test Center" />
        <NavItem icon={<Database size={22} />} active={activeTab === "data"} onClick={() => setActiveTab("data")} label="Batch Data" />
        <NavItem icon={<FileText size={22} />} active={activeTab === "reports"} onClick={() => setActiveTab("reports")} label="Report Center" />
        <NavItem icon={<Settings size={22} />} active={activeTab === "settings"} onClick={() => setActiveTab("settings")} label="Configuration" />
        
        <div className="w-full h-[1px] bg-slate-800 my-2" />
        <button
          onClick={startDemo}
          disabled={isDemoActive || isRunning}
          className={`w-12 h-12 rounded-xl flex items-center justify-center transition-all group relative ${
            isDemoActive ? "bg-amber-600 text-white shadow-lg" : "text-amber-500 hover:bg-amber-500/10 hover:text-amber-400"
          } disabled:opacity-50`}
        >
          <Zap size={22} fill={isDemoActive ? "currentColor" : "none"} />
          <div className="absolute left-16 bg-amber-600 text-white text-[9px] px-2 py-1.5 rounded font-black opacity-0 group-hover:opacity-100 transition-all transform translate-x-[-10px] group-hover:translate-x-0 whitespace-nowrap pointer-events-none uppercase tracking-widest z-50 shadow-2xl">
            Run Demo Cycle
          </div>
        </button>

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
              <div className="flex flex-col">
                <span className="text-[8px] font-black text-slate-500 uppercase">Current Method</span>
                <span className="text-[10px] font-bold text-slate-300 uppercase tracking-widest">{testMethod}</span>
              </div>
            </div>
          </div>
          <div className="flex items-center gap-4">
            <div className="flex items-center gap-4 px-4 py-1 bg-slate-900/50 rounded-full border border-slate-700 text-[10px] font-bold">
              <div className="flex items-center gap-1.5">
                <div className="w-1.5 h-1.5 rounded-full bg-blue-500 animate-pulse" />
                <span className="text-slate-300">CELL: {loadCell}</span>
              </div>
              <div className="w-[1px] h-3 bg-slate-700" />
              <div className="flex items-center gap-1.5" onClick={() => setIsSafetyDoorClosed(!isSafetyDoorClosed)} style={{ cursor: 'pointer' }}>
                <div className={`w-1.5 h-1.5 rounded-full ${isSafetyDoorClosed ? 'bg-green-500' : 'bg-red-500 animate-ping'}`} />
                <span className={isSafetyDoorClosed ? 'text-green-400' : 'text-red-400'}>{isSafetyDoorClosed ? "DOOR CLOSED" : "DOOR OPEN"}</span>
              </div>
            </div>
            <Button variant="ghost" size="sm" className="h-8 text-xs text-slate-400 hover:text-white" onClick={() => toast.success("Batch saved to memory")}><Save size={14} className="mr-2" /> Save</Button>
            <Button variant="ghost" size="sm" className="h-8 text-xs text-slate-400 hover:text-white" onClick={() => handleExport()}><Download size={14} className="mr-2" /> Export</Button>
          </div>
        </header>

        <div className="flex-1 flex overflow-hidden">
          {activeTab === "test" && (
            <>
              <div className="w-80 bg-[#1e293b] border-r border-slate-700 flex flex-col">
                <Tabs defaultValue="samples" className="flex-1 flex flex-col overflow-hidden">
                  <TabsList className="bg-slate-900/50 h-10 p-1 border-b border-slate-700 rounded-none w-full">
                    <TabsTrigger value="samples" className="flex-1 text-[9px] font-black uppercase h-8">Samples</TabsTrigger>
                    <TabsTrigger value="params" className="flex-1 text-[9px] font-black uppercase h-8">Method</TabsTrigger>
                    <TabsTrigger value="history" className="flex-1 text-[9px] font-black uppercase h-8">Batch</TabsTrigger>
                  </TabsList>
                  
                  <TabsContent value="samples" className="flex-1 flex flex-col overflow-hidden m-0">
                    <div className="p-3 flex items-center justify-between bg-slate-900/20 border-b border-slate-700">
                      <h2 className="text-[10px] font-black uppercase tracking-tighter text-slate-400">Specimen Queue</h2>
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
                                ? 'bg-blue-600/20 border-blue-500/50' 
                                : 'bg-slate-800/40 border-slate-700'
                            }`}
                          >
                            <div className="flex justify-between items-start mb-1">
                              <span className={`text-[10px] font-black ${activeSampleId === sample.id ? 'text-blue-300' : 'text-slate-400'}`}>ID: {sample.id}</span>
                              <span className={`text-[8px] px-1.5 py-0.5 rounded-sm font-black uppercase border ${
                                sample.status === 'Completed' ? 'bg-green-500/10 text-green-400 border-green-500/30' : 'bg-slate-700 border-slate-600 text-slate-400'
                              }`}>{sample.status}</span>
                            </div>
                            <span className={`text-xs font-bold block mb-1 ${activeSampleId === sample.id ? 'text-white' : 'text-slate-300'}`}>{sample.name}</span>
                            <div className="text-[10px] text-slate-500 flex justify-between items-center font-mono">
                              <span>S: {sample.result}</span>
                              <button onClick={(e) => { e.stopPropagation(); deleteSample(sample.id); }} className="opacity-0 group-hover:opacity-100 p-1 hover:text-red-400"><Trash2 size={10} /></button>
                            </div>
                          </div>
                        ))}
                      </div>
                    </ScrollArea>
                  </TabsContent>
                  
                  <TabsContent value="params" className="flex-1 p-4 space-y-4 m-0 bg-slate-900/20 overflow-auto">
                    <h2 className="text-[10px] font-black uppercase text-slate-400">Geometry Settings</h2>
                    <div className="space-y-4">
                      <div className="grid grid-cols-2 gap-3">
                        <div className="space-y-1.5">
                          <Label className="text-[9px] font-bold text-slate-500 uppercase">Width (mm)</Label>
                          <Input value={activeSample.width} onChange={(e) => updateSampleParam(activeSampleId, 'width', e.target.value)} className="h-8 bg-slate-800 border-slate-700 text-xs font-mono" />
                        </div>
                        <div className="space-y-1.5">
                          <Label className="text-[9px] font-bold text-slate-500 uppercase">Thickness (mm)</Label>
                          <Input value={activeSample.thickness} onChange={(e) => updateSampleParam(activeSampleId, 'thickness', e.target.value)} className="h-8 bg-slate-800 border-slate-700 text-xs font-mono" />
                        </div>
                      </div>
                    </div>
                  </TabsContent>

                  <TabsContent value="history" className="flex-1 p-4 m-0 overflow-auto bg-slate-900/30">
                    <h2 className="text-[10px] font-black uppercase text-slate-400 mb-4 tracking-widest">Recent Batch Data</h2>
                    <div className="space-y-2">
                       <div className="p-2 bg-slate-800/60 rounded border border-slate-700">
                         <span className="text-[9px] font-black text-slate-500 uppercase block mb-1">Batch Avg Stress</span>
                         <span className="text-lg font-black text-white">46.2 MPa</span>
                       </div>
                    </div>
                  </TabsContent>
                </Tabs>
              </div>

              <div className="flex-1 flex flex-col relative">
                <div className="h-40 bg-[#020617] flex border-b border-slate-800 divide-x divide-slate-800 shadow-2xl overflow-x-auto">
                  <BigMeter label="Force" value={liveMetrics.force.toFixed(2)} unit="N" color="text-blue-500" />
                  <BigMeter label="Extension" value={liveMetrics.extension.toFixed(3)} unit="mm" color="text-emerald-500" />
                  <BigMeter label="Stress" value={liveMetrics.stress.toFixed(2)} unit="MPa" color="text-amber-500" />
                  <BigMeter label="Strain" value={liveMetrics.strain.toFixed(2)} unit="%" color="text-purple-500" />
                  
                  <div className="min-w-[200px] flex-1 flex flex-col items-center justify-center p-4 bg-slate-900/20 gap-3">
                    <Button 
                      onClick={isRunning ? () => handleStop(false) : handleStart}
                      className={`w-full h-14 text-xl font-black gap-3 shadow-2xl transition-all active:scale-95 ${
                        isRunning 
                          ? 'bg-red-600 hover:bg-red-700' 
                          : 'bg-green-600 hover:bg-green-700'
                      }`}
                    >
                      {isRunning ? <Square size={24} fill="currentColor" /> : <Play size={24} fill="currentColor" />}
                      {isRunning ? "HALT" : "START"}
                    </Button>
                  </div>
                </div>

                <div className="flex-1 p-4 grid grid-rows-[1fr_200px] gap-4 overflow-hidden">
                  <Card className="bg-[#1e293b] border-slate-700 shadow-2xl flex flex-col relative overflow-hidden">
                    <div className="px-4 py-2 border-b border-slate-700 flex justify-between items-center bg-slate-900/40 z-10">
                      <span className="text-[10px] font-black uppercase text-blue-400 flex items-center gap-2"><Activity size={12}/> Live Curve Display</span>
                    </div>
                    <div className="flex-1 p-4 relative">
                      <ResponsiveContainer width="100%" height="100%">
                        <LineChart data={currentData}>
                          <CartesianGrid strokeDasharray="3 3" vertical={true} stroke="#334155" />
                          <XAxis dataKey="strain" type="number" domain={[0, 'auto']} tick={{fontSize: 9, fill: '#64748b'}} />
                          <YAxis tick={{fontSize: 9, fill: '#64748b'}} />
                          <Tooltip contentStyle={{ backgroundColor: '#020617', border: '1px solid #334155', fontSize: '10px' }} />
                          <Line type="monotone" dataKey="stress" stroke="#3b82f6" strokeWidth={3} dot={false} isAnimationActive={false} />
                        </LineChart>
                      </ResponsiveContainer>
                    </div>
                  </Card>

                  <div className="grid grid-cols-4 gap-4">
                    <ValueCard label="Tensile Strength" value={isRunning ? (liveMetrics.stress * 1.05).toFixed(1) : (activeSample.status === "Completed" ? activeSample.result.split(' ')[0] : "0.0")} unit="MPa" icon={<Activity size={14}/>} />
                    <ValueCard label="Load @ Yield" value={isRunning ? (liveMetrics.force * 0.9).toFixed(2) : "0.00"} unit="N" icon={<Gauge size={14}/>} />
                    <ValueCard label="Crosshead Speed" value={liveMetrics.speed.toFixed(3)} unit="mm/min" icon={<Zap size={14}/>} />
                    <ValueCard label="Internal Temp" value={liveMetrics.temp.toFixed(1)} unit="°C" icon={<Thermometer size={14}/>} />
                  </div>
                </div>
              </div>

              <div className="w-64 bg-[#1e293b] border-l border-slate-700 flex flex-col">
                <div className="p-4 border-b border-slate-700 bg-slate-900/10">
                  <h2 className="text-[10px] font-black uppercase tracking-tighter text-slate-400 mb-4 flex items-center gap-2"><Monitor size={12}/> Machine IO</h2>
                  <div className="grid grid-cols-2 gap-2">
                    <HardwareBtn icon={<ChevronDown size={16} />} label="Jog Down" onClick={() => handleJog('down')} />
                    <HardwareBtn icon={<ChevronDown className="rotate-180" size={16} />} label="Jog Up" onClick={() => handleJog('up')} />
                    <HardwareBtn icon={<Monitor size={16} />} label="Zero Load" onClick={() => { setLiveMetrics(m => ({...m, force: 0, stress: 0})); toast.success("Load Cell Zeroed"); }} />
                    <HardwareBtn icon={<Monitor size={16} />} label="Zero Ext" onClick={() => { setLiveMetrics(m => ({...m, extension: 0, strain: 0})); toast.success("Extension Zeroed"); }} />
                  </div>
                </div>
                
                <div className="p-4 flex-1 space-y-6">
                  <h2 className="text-[10px] font-black uppercase tracking-tighter text-slate-400 flex items-center gap-2"><Layers size={12}/> Logic Stack</h2>
                  <div className="space-y-4">
                    <StatusStep label="Crosshead Interlock" status={isSafetyDoorClosed ? "Done" : "Fault"} error={!isSafetyDoorClosed} />
                    <StatusStep label="Grip Engagement" status={sequenceStatus.grip} />
                    <StatusStep label="Slack Removal" status={sequenceStatus.slack} />
                    <StatusStep label="Test Acquisition" status={sequenceStatus.main} active={isRunning} />
                    <StatusStep label="Data Analysis" status={sequenceStatus.peak} />
                  </div>
                </div>
              </div>
            </>
          )}

          {activeTab === "reports" && (
            <div className="flex-1 p-8 bg-slate-950 overflow-auto">
              <div className="max-w-6xl mx-auto space-y-8">
                <div className="flex justify-between items-end border-b border-slate-800 pb-6">
                  <div>
                    <h2 className="text-3xl font-black text-white tracking-tighter uppercase italic">REPORT ARCHIVE</h2>
                    <p className="text-slate-500 font-bold uppercase text-[10px] tracking-widest mt-1">Generated Batch Analysis Documents</p>
                  </div>
                </div>
                <div className="grid grid-cols-1 gap-4">
                  {samples.filter(s => s.status === "Completed").length === 0 ? (
                    <div className="p-20 text-center border border-dashed border-slate-800 rounded-2xl bg-slate-900/20">
                      <FileText size={48} className="mx-auto text-slate-700 mb-4" />
                      <h3 className="text-lg font-bold text-slate-400">No Reports Available</h3>
                      <p className="text-sm text-slate-600 mt-2">Complete a test to generate a sample report.</p>
                    </div>
                  ) : (
                    samples.filter(s => s.status === "Completed").map(sample => (
                      <Card key={sample.id} className="bg-[#1e293b] border-slate-700 p-6 flex items-center justify-between group hover:border-blue-500/50 transition-all">
                        <div className="flex items-center gap-4">
                          <div className="w-12 h-12 bg-blue-600/10 rounded-lg flex items-center justify-center text-blue-500 border border-blue-500/20">
                            <FileText size={24} />
                          </div>
                          <div>
                            <h4 className="font-bold text-white">{sample.name} - Analysis Report</h4>
                            <div className="flex gap-4 mt-1">
                              <span className="text-[10px] font-black text-slate-500 flex items-center gap-1 uppercase tracking-wider"><Clock size={10}/> {sample.date || '2025-12-25'}</span>
                              <span className="text-[10px] font-black text-blue-400 flex items-center gap-1 uppercase tracking-wider"><CheckCircle2 size={10}/> ISO 527 COMPLIANT</span>
                            </div>
                          </div>
                        </div>
                        <div className="flex items-center gap-3">
                          <div className="text-right mr-4">
                            <p className="text-[10px] font-black text-slate-500 uppercase">Peak Strength</p>
                            <p className="text-lg font-black text-blue-400 font-mono leading-none">{sample.result}</p>
                          </div>
                          <Button variant="ghost" className="h-10 px-4 text-xs font-black uppercase hover:bg-blue-600 hover:text-white" onClick={() => handleExport(sample)}><Download size={14} className="mr-2"/> PDF Export</Button>
                        </div>
                      </Card>
                    ))
                  )}
                </div>
              </div>
            </div>
          )}

          {activeTab === "data" && (
            <div className="flex-1 p-8 bg-slate-950 overflow-auto">
              <div className="max-w-6xl mx-auto space-y-8">
                <div className="flex justify-between items-end border-b border-slate-800 pb-6">
                  <div>
                    <h2 className="text-3xl font-black text-white tracking-tighter uppercase italic">LIMS Database</h2>
                    <p className="text-slate-500 font-bold uppercase text-[10px] tracking-widest mt-1">Laboratory Information Management System</p>
                  </div>
                </div>
                <div className="grid grid-cols-4 gap-4">
                  <StatBox label="Database Entries" value={samples.length.toString()} icon={<Database size={20}/>} />
                  <StatBox label="Successful Runs" value={samples.filter(s => s.status === "Completed").length.toString()} icon={<Activity size={20}/>} color="text-green-400" />
                  <StatBox label="Last Updated" value="TODAY" icon={<History size={20}/>} />
                  <StatBox label="Storage Load" value="12%" icon={<Monitor size={20}/>} />
                </div>
                <Card className="bg-[#1e293b] border-slate-700 overflow-hidden shadow-2xl">
                  <table className="w-full text-left">
                    <thead className="bg-slate-900/80 text-[10px] font-black uppercase text-slate-500 border-b border-slate-700">
                      <tr>
                        <th className="px-6 py-4">Sample Identifier</th>
                        <th className="px-6 py-4 text-center">Ultimate Strength</th>
                        <th className="px-6 py-4">Status</th>
                        <th className="px-6 py-4">Operator</th>
                        <th className="px-6 py-4 text-right">Actions</th>
                      </tr>
                    </thead>
                    <tbody className="divide-y divide-slate-800">
                      {samples.map(sample => (
                        <tr key={sample.id} className="hover:bg-blue-600/5 transition-colors group">
                          <td className="px-6 py-4 text-xs font-bold text-slate-300">{sample.name}</td>
                          <td className="px-6 py-4 text-xs font-mono text-blue-400 text-center">{sample.result}</td>
                          <td className="px-6 py-4">
                             <span className={`text-[8px] px-2 py-0.5 rounded-sm font-black uppercase border ${
                              sample.status === 'Completed' ? 'bg-green-500/10 text-green-400 border-green-500/30' : 'bg-slate-800 border-slate-700 text-slate-500'
                            }`}>{sample.status}</span>
                          </td>
                          <td className="px-6 py-4 text-[10px] font-bold text-slate-500 uppercase">{sample.operator || 'ADMIN'}</td>
                          <td className="px-6 py-4 text-right">
                             <div className="flex justify-end gap-2">
                               <Button variant="ghost" size="icon" className="h-7 w-7 text-slate-500 hover:text-blue-400"><Eye size={14}/></Button>
                               <Button variant="ghost" size="icon" className="h-7 w-7 text-slate-500 hover:text-green-400" onClick={() => handleExport(sample)}><Download size={14}/></Button>
                             </div>
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
              <div className="max-w-5xl mx-auto space-y-10">
                <div className="border-b border-slate-800 pb-6">
                  <h2 className="text-3xl font-black text-white tracking-tighter uppercase italic flex items-center gap-3">
                    <Settings size={32} className="text-blue-500" />
                    System Configuration
                  </h2>
                  <p className="text-slate-500 font-bold uppercase text-[10px] tracking-widest mt-1">Hardware and Method Administration</p>
                </div>

                <div className="grid grid-cols-2 gap-8">
                  {/* Hardware Block */}
                  <Card className="bg-[#1e293b] border-slate-700 p-6 space-y-6 shadow-xl">
                    <div className="flex items-center gap-2 text-blue-400">
                      <Cpu size={18} />
                      <h3 className="text-xs font-black uppercase tracking-widest">Hardware Registry</h3>
                    </div>
                    
                    <div className="space-y-4">
                      <div className="space-y-2">
                        <Label className="text-[10px] font-black text-slate-500 uppercase">Load Cell Interface</Label>
                        <select 
                          className="w-full h-10 bg-slate-900 border-slate-700 rounded-md px-3 text-xs font-bold text-slate-300 outline-none focus:ring-2 ring-blue-500"
                          value={loadCell}
                          onChange={(e) => setLoadCell(e.target.value)}
                        >
                          <option>STDM-100kN (S/N 8821)</option>
                          <option>STDM-50kN (S/N 4412)</option>
                          <option>STDM-10kN (S/N 2102)</option>
                        </select>
                      </div>

                      <div className="space-y-2">
                        <Label className="text-[10px] font-black text-slate-500 uppercase">Frame Status</Label>
                        <select 
                          className="w-full h-10 bg-slate-900 border-slate-700 rounded-md px-3 text-xs font-bold text-slate-300 outline-none focus:ring-2 ring-blue-500"
                          value={machineStatus}
                          onChange={(e) => setMachineStatus(e.target.value)}
                        >
                          <option>Online</option>
                          <option>Offline</option>
                          <option>Simulation Mode</option>
                        </select>
                      </div>
                    </div>

                    <div className="p-4 bg-slate-900/50 rounded-lg border border-slate-800 space-y-3">
                      <div className="flex justify-between items-center text-[10px] font-bold">
                        <span className="text-slate-500 uppercase">Firmware</span>
                        <span className="text-slate-300">v4.8.2-PRO</span>
                      </div>
                      <div className="flex justify-between items-center text-[10px] font-bold">
                        <span className="text-slate-500 uppercase">Comm Link</span>
                        <span className="text-green-500">ETHERNET/IP</span>
                      </div>
                      <Button variant="outline" className="w-full h-8 text-[10px] font-black border-slate-700 uppercase">Sync Hardware</Button>
                    </div>
                  </Card>

                  {/* Security Block */}
                  <Card className="bg-[#1e293b] border-slate-700 p-6 space-y-6 shadow-xl">
                    <div className="flex items-center gap-2 text-amber-500">
                      <Shield size={18} />
                      <h3 className="text-xs font-black uppercase tracking-widest">Security & Interlocks</h3>
                    </div>

                    <div className="space-y-4">
                      <div className="flex items-center justify-between p-4 bg-slate-900/30 border border-slate-800 rounded-xl">
                        <div className="flex items-center gap-3">
                          <Lock size={20} className={isSafetyDoorClosed ? "text-green-500" : "text-red-500"} />
                          <div>
                            <p className="text-xs font-black text-white uppercase tracking-tight">Safety Door Sensor</p>
                            <p className="text-[9px] font-bold text-slate-500">{isSafetyDoorClosed ? "Active & Locked" : "Warning: Interlock Bypassed"}</p>
                          </div>
                        </div>
                        <Button 
                          variant={isSafetyDoorClosed ? "outline" : "destructive"} 
                          size="sm" 
                          className="h-8 text-[9px] font-black uppercase"
                          onClick={() => setIsSafetyDoorClosed(!isSafetyDoorClosed)}
                        >
                          {isSafetyDoorClosed ? "Open Door" : "Close Door"}
                        </Button>
                      </div>

                      <div className="flex items-center justify-between p-4 bg-slate-900/30 border border-slate-800 rounded-xl opacity-50">
                        <div className="flex items-center gap-3">
                          <Radio size={20} className="text-blue-500" />
                          <div>
                            <p className="text-xs font-black text-white uppercase tracking-tight">Remote LIMS Sync</p>
                            <p className="text-[9px] font-bold text-slate-500">Cloud Data Mirroring</p>
                          </div>
                        </div>
                        <div className="text-[8px] font-black text-slate-600 border border-slate-700 px-2 py-1 rounded">DISABLED</div>
                      </div>
                    </div>

                    <div className="p-4 bg-amber-500/10 border border-amber-500/20 rounded-xl">
                      <div className="flex items-start gap-2">
                        <AlertTriangle size={14} className="text-amber-500 mt-0.5" />
                        <p className="text-[10px] font-bold text-amber-200 leading-tight uppercase tracking-tight">
                          Calibration expires in 172 days. Schedule professional verification to maintain ISO compliance.
                        </p>
                      </div>
                    </div>
                  </Card>
                </div>
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
    <div className="flex flex-col justify-center px-8 border-r border-slate-800 transition-colors min-w-[180px]">
      <span className="text-[9px] font-black text-slate-500 uppercase mb-1 tracking-tighter">{label}</span>
      <div className="flex items-baseline gap-2">
        <span className={`text-5xl font-mono font-black tabular-nums ${color}`}>{value}</span>
        <span className="text-xs font-black text-slate-600 uppercase">{unit}</span>
      </div>
    </div>
  );
}

function ValueCard({ label, value, unit, icon, color = "text-white" }: { label: string; value: string; unit: string; icon?: React.ReactNode; color?: string }) {
  return (
    <div className="bg-[#1e293b] p-4 rounded-xl border border-slate-700 shadow-xl group hover:border-blue-500/50 transition-all relative overflow-hidden">
      <div className="absolute -right-2 -bottom-2 opacity-5 group-hover:opacity-10 transition-opacity text-white scale-[2.5]">{icon}</div>
      <div className="flex justify-between items-start mb-2">
        <span className="text-[9px] font-black text-slate-500 uppercase tracking-widest group-hover:text-blue-400">{label}</span>
        <div className="text-blue-500/50">{icon}</div>
      </div>
      <div className="flex items-baseline gap-2">
        <span className={`text-2xl font-black tabular-nums tracking-tighter ${color}`}>{value}</span>
        <span className="text-[10px] font-black text-slate-600 uppercase">{unit}</span>
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
      <span className="text-[8px] font-black uppercase text-slate-500 group-hover:text-white leading-none">{label}</span>
    </Button>
  );
}

function StatusStep({ label, status, active, error }: { label: string; status: string; active?: boolean; error?: boolean }) {
  const getStatusColor = () => {
    if (error) return "bg-red-500 border-red-400";
    if (status === "Done") return "bg-green-500 border-green-400";
    if (status === "Active" || active) return "bg-blue-500 border-blue-400 animate-pulse ring-2 ring-blue-500/20";
    return "bg-slate-800 border-slate-700";
  };

  const getTextColor = () => {
    if (error) return "text-red-400";
    if (status === "Done") return "text-slate-400";
    if (status === "Active" || active) return "text-blue-400";
    return "text-slate-600";
  };

  return (
    <div className="flex items-center gap-3 group">
      <div className={`w-2 h-2 rounded-full border ${getStatusColor()}`} />
      <div className="flex flex-col">
        <span className={`text-[10px] font-bold uppercase tracking-tight ${getTextColor()}`}>{label}</span>
        <span className={`text-[7px] font-black ${error ? 'text-red-500' : 'text-slate-500'}`}>{status}</span>
      </div>
    </div>
  );
}

function StatBox({ label, value, icon, color = "text-white" }: { label: string; value: string; icon?: React.ReactNode; color?: string }) {
  return (
    <div className="bg-[#1e293b] p-6 rounded-2xl border border-slate-700 shadow-xl flex items-center justify-between group hover:border-blue-500/50 transition-all">
      <div className="space-y-2">
        <span className="text-[10px] font-black text-slate-500 uppercase tracking-widest block">{label}</span>
        <div className="flex items-baseline gap-1">
          <span className={`text-4xl font-black tracking-tighter ${color}`}>{value}</span>
        </div>
      </div>
      <div className="text-slate-700 group-hover:text-blue-500/30 transition-colors scale-[1.5]">{icon}</div>
    </div>
  );
}
