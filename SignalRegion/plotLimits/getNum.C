void getNum(){
  string str = "/uscms/home/vhegde/nobackup/Physics/skims_Run2ProductionV12/SignalRegion_GetLimits/condor_T5bbbbZg_FastSim_2500_2450_job.stdout:Limit: r < 21.5474 @ 95% CL";

    // std::string str2 = str.substr (3,5);     // "think"

    // std::size_t pos = str.find("live");      // position of "live" in str

    // std::string str3 = str.substr (pos);     // get from "live" to the end

    // std::cout << str2 << ' ' << str3 << '\n';


  ifstream filein("runList_T5qqqqHg.txt");
  string line="condor_T5bbbbZg_FastSim_2500_2450_job.stdout:Limit: r < 21.5474 @ 95% CL";
  string strGl = line.substr(line.find("FastSim_")+8, line.find(");
  cout<<strGl<<endl;
  // while (getline(filein, line)) {
  //   stringstream ss(line);
  // }
  
}
