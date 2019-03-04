#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <iomanip>
#include <cmath>
#include <istream>
#include <time.h>

void PrintMatrix (std::vector<std::vector<long double> > matrix, std::string name)
{
  std::ofstream file;
  file.open(name, std::ofstream::out);
  for (std::vector<std::vector<long double> >::iterator i = matrix.begin(); i != matrix.end(); ++i)
  {
    std::vector<long double>::iterator j = (*i).begin();
    file << (*j);
    j++;
    for (; j != (*i).end(); ++j)
    {
      file << "," << (*j);
    }
    file << std::endl;
  }
  file.close();
}

std::vector<int> DA_Rangarajan (std::vector<std::vector<long double> > b_ij, int N) {
  // normalize bij
  double max = 0;
  for (unsigned i = 0; i < N; ++i) // UAV
  {
    for (unsigned j = 0; j < N; ++j) // LOC
    {
      if (b_ij[i][j] > max) {
        max = b_ij[i][j];
      }
    }
  }
  for (unsigned i = 0; i < N; ++i) // UAV
  {
    for (unsigned j = 0; j < N; ++j) // LOC
    {
      b_ij[i][j] /= max;
    }
  }

  // srand(time(NULL));
  double temp = 0.9;
  std::vector<int> proposed_FINAL;
  std::vector<int> proposed_UAV;
  std::vector<int> proposed_LOC;
  std::vector<long double> z;
  std::vector<long double> pun_v;
  // Mai
  std::vector<std::vector<long double> > m_ij;
  // variavel da tranformacao algebrica (parte do self-amplification)
  std::vector<std::vector<long double> > o_ij;
  // variavel da transformacao algebrica (parte o Y_sch)
  std::vector<std::vector<long double> > lamb_ij;
  // q_ai
  std::vector<std::vector<long double> > q_ij;
  // Ptr<UniformRandomVariable> e_ai = CreateObject<UniformRandomVariable>(); // Padrão [0,1]
  // e_ai->SetAttribute ("Min", DoubleValue (min));
  // double rdom;

  for (unsigned i = 0; i < N; ++i) // UAV
  {
    m_ij.push_back(std::vector<long double>());
    o_ij.push_back(std::vector<long double>());
    lamb_ij.push_back(std::vector<long double>());
    q_ij.push_back(std::vector<long double>());
    for (unsigned j = 0; j < N; ++j) // LOC
    {
      // rdom = (double)rand()/RAND_MAX; // [0,1]
      m_ij[i].push_back(1.0/(double)N); // + rdom);
      o_ij[i].push_back(m_ij[i][j]);
      lamb_ij[i].push_back(0.0);
      q_ij[i].push_back(0.0);
    }
    proposed_FINAL.push_back(-1);
    proposed_UAV.push_back(-1);
    proposed_LOC.push_back(-1);
    z.push_back(0.0);
    pun_v.push_back(0.0);
  }

  long double gamma = 0.6; // o que faz?
  long double lamb = 1;
  long double new_mij;
  int odd_even = 0;
  unsigned i, j, k;
  int m_pos;
  long double v_max;
  int check;

  std::cout << std::fixed << std::setw(8) << std::setprecision(8);

  while (temp >= 1e-5)
  {
    std::cout << "---------------------------- temp: " << temp << "  odd_even: " << odd_even << std::endl;

    if ((odd_even%2)==0) { // line total -- UAV
      for (i = 0; i < N; ++i)
      {   
        pun_v[i] = 0.0;
        for (k = 0; k < N; ++k)
        {
          pun_v[i] += m_ij[i][k]; // sum line -- hold line, change column
        }
        z[i] = 0.0;
        for (k = 0; k < N; ++k)
        {
          z[i] += expl((gamma * o_ij[i][k] - m_ij[i][k] * b_ij[i][k] * b_ij[i][k] + ((pun_v[i]>1)?(1-temp)*lamb : 0)) / (long double) temp); // hold line, change column          
        }
      }
    } else {  // column total -- LOC
      for (i = 0; i < N; ++i)
      {    
        pun_v[i] = 0.0;
        for (k = 0; k < N; ++k)
        {
          pun_v[i] += m_ij[k][i]; // sum column - hold column, change line
        }
        z[i] = 0.0;
        for (k = 0; k < N; ++k)
        {
          z[i] += expl((gamma * o_ij[k][i] - m_ij[k][i] * b_ij[k][i] * b_ij[k][i] + ((pun_v[i]>1)?(1-temp)*lamb : 0)) / (long double) temp); // hold column, change lines
        }
      }
    }

    { // frufru
      std::cout << "...... Z \n[";
      for (i = 0; i < N; ++i)
      {
        std::cout << z[i] << "\t\t";
      }
      std::cout << "]\n";

      std::cout << "...... PUN V \n[";
      for (i = 0; i < N; ++i)
      {
        std::cout << pun_v[i] << "\t\t";
      }
      std::cout << "]\n";

      std::cout << "...... BIJ\n";
      for (i = 0; i < N; ++i)
      {
        std::cout << "[";
        for (j = 0; j < N; ++j)
        {
          std::cout << b_ij[i][j] << "\t\t";
        }  
        std::cout << "]\n";    
      }   
      
      std::cout << "...... MIJ\n";
      for (i = 0; i < N; ++i)
      {
        std::cout << "[";
        for (j = 0; j < N; ++j)
        {
          std::cout << m_ij[i][j] << "\t\t";
        }  
        std::cout << "]\n";    
      }    

      std::cout << "...... m_ij[i][j] * b_ij[i][j]\n";
      for (i = 0; i < N; ++i)
      {
        std::cout << "[";
        for (j = 0; j < N; ++j)
        {
          std::cout << m_ij[i][j] * b_ij[i][j] << "\t\t";
        }  
        std::cout << "]\n";    
      }

      std::cout << "...... m_ij[i][j] * b_ij[i][j]  * b_ij[i][j]\n";
      for (i = 0; i < N; ++i)
      {
        std::cout << "[";
        for (j = 0; j < N; ++j)
        {
          std::cout << (m_ij[i][j] * b_ij[i][j]  * b_ij[i][j]) << "\t\t";
        }  
        std::cout << "]\n";    
      }

      std::cout << "...... gamma * o_ij[i][j]\n";
      for (i = 0; i < N; ++i)
      {
        std::cout << "[";
        for (j = 0; j < N; ++j)
        {
          std::cout << gamma*o_ij[i][j] << "\t\t";
        }  
        std::cout << "]\n";    
      }

      std::cout << "...... gamma * o_ij[i][j] - m_ij[i][j] * b_ij[i][j]  * b_ij[i][j]\n";
      for (i = 0; i < N; ++i)
      {
        std::cout << "[";
        for (j = 0; j < N; ++j)
        {
          std::cout << gamma * o_ij[i][j] - (m_ij[i][j] * b_ij[i][j]  * b_ij[i][j]) + ((odd_even%2==0 && pun_v[i]>1) ? (1-temp)*lamb : ((odd_even%2!=0 && pun_v[j]>1) ? (1-temp)*lamb : 0)) << "\t\t";
        }  
        std::cout << "]\n";    
      }

      std::cout << "...... QIJ/temp\n";
      for (i = 0; i < N; ++i)
      {
        std::cout << "[";
        for (j = 0; j < N; ++j)
        {
          std::cout << ((gamma * o_ij[i][j] - (m_ij[i][j] * b_ij[i][j]  * b_ij[i][j]) +((odd_even%2==0 && pun_v[i]>1) ? (1-temp)*lamb : ((odd_even%2!=0 && pun_v[j]>1) ? (1-temp)*lamb : 0)))/(long double)temp) << "\t\t";
        }  
        std::cout << "]\n";    
      }

      std::cout << "...... exp QIJ/temp\n";
      for (i = 0; i < N; ++i)
      {
        std::cout << "[";
        for (j = 0; j < N; ++j)
        {
          std::cout << expl((gamma * o_ij[i][j]-(m_ij[i][j] * b_ij[i][j]  * b_ij[i][j])+((odd_even%2==0 && pun_v[i]>1) ? (1-temp)*lamb : ((odd_even%2!=0 && pun_v[j]>1) ? (1-temp)*lamb : 0)))/(long double)temp) << "\t\t";
        }  
        std::cout << "]\n";    
      }    
    }

    for (i = 0; i < N; ++i)
    {
      for (j = 0; j < N; ++j)
      {
        // calculate \lamb_{ij}
        lamb_ij[i][j] = m_ij[i][j] * b_ij[i][j];
        // calculate Q_{ij}
        q_ij[i][j] =  gamma * o_ij[i][j] - lamb_ij[i][j] * b_ij[i][j];        
        // calculate m_{ij}
        new_mij = expl((q_ij[i][j] + ((odd_even%2==0 && pun_v[i]>1) ? (1-temp)*lamb : ((odd_even%2!=0 && pun_v[j]>1) ? (1-temp)*lamb : 0)) ) / (long double) temp) / ((odd_even%2==0) ? z[i] : z[j]); // apply normalization here, the change from line/column occurs above in the correct calculation of Z
        if (isnan(new_mij)) {
          std::cout << "NAN! \n";
          exit(1);
        }
        m_ij[i][j] = new_mij;
      }
    }    

    { // validate MIJ
      if (odd_even%2==0) {
        long double validate;
        std::cout << "...... MIJ --------- line\n";
        for (i = 0; i < N; ++i)
        {
          validate = 0.0;
          std::cout << "[";
          for (j = 0; j < N; ++j)
          {
            std::cout << m_ij[i][j] << "\t\t";
            validate += m_ij[i][j];
          }  
          std::cout << "] = " << validate << "\n";    
        }
      } else {
        std::cout << "...... MIJ --------- column\n";
        std::vector<long double> validate(N,0);
        for (i = 0; i < N; ++i)
        {
          std::cout << "[";
          for (j = 0; j < N; ++j)
          {
            std::cout << m_ij[i][j] << "\t\t";
            validate[i] += m_ij[j][i];
          }  
          std::cout << "]\n";    
        }
        std::cout << "---------------------------------------------------------\n";
        std::cout << "[";
        for (i = 0; i < N; ++i)
        {
          std::cout << validate[i] << "\t\t";
        }
        std::cout << "]\n";
      }
    }

    std::cout << "UAV \n[";
    for(i = 0; i < N; i++)
    {
      std::cout << proposed_UAV[i] << "\t\t";
    }
    std::cout << "]\n";

    std::cout << "LOC \n[";
    for(i = 0; i < N; i++)
    {
      std::cout << proposed_LOC[i] << "\t\t";
    }
    std::cout << "]\n";

    if (check == N) {
      goto out;
    }

    temp *= 0.99;
    o_ij = m_ij;
    odd_even++;
    std::cout << " ---------------------------------------- \n";
  }
  // permite sair dos lacos ao encontrar 1 para cada localizacao
  out:
  
  return proposed_FINAL;
}

std::vector<int> da_positioning (std::vector<std::vector<long double> > c_ji, int N) { //, std::string path) {
  long double temp = 0.3, t_min=9e-6, alpha, max, cost, cost_FINAL = 5.0; // alpha punicao de capacidade
  int check, p_max;

  int odd_even = 0;
  alpha = 1.05; // aumenta 20%
  unsigned uav, loc;
  bool equal;
  
  std::vector<std::vector<long double> > b_ji;
  std::vector<std::vector<long double> > m_ji;
  std::vector<long double> o_punish; // ocupada
  std::vector<long double> z;
  std::vector<long double> validate;
  std::vector<int> used; 
  std::vector<int> o_conflict;
  std::vector<int> proposed_FINAL;
  std::vector<int> proposed_LOC;
  std::vector<int> proposed_UAV;

  for(int j = 0; j < N; j++) // uav
  {
    m_ji.push_back(std::vector<long double>());
    b_ji.push_back(std::vector<long double>());
    max = 0.0;
    for(int i = 0; i < N; i++) // location
    {
      m_ji[j].push_back(0.01);// para nao dar conflito com a primeira atualizaçao de bij
      b_ji[j].push_back(c_ji[j][i]);
      if (b_ji[j][i] > max) {
        max = b_ji[j][i];
      }
    }
    for (int i = 0; i < N; ++i) 
    {
      b_ji[j][i] /= max;
    }
    o_punish.push_back(1e-1);
    proposed_LOC.push_back(-1);
    proposed_UAV.push_back(-1);
    proposed_FINAL.push_back(-1);
    used.push_back(0);
    o_conflict.push_back(1); // para saber se ja teve conflito 
  }

  std::cout << "============================> STARTING\n";
  while (temp > t_min) {
    // alpha = 1-temp;
    std::cout << "############################################################################### TEMP = " << temp << "  ALPHA = " << alpha << " ODDEVEN = " << odd_even << std::endl;   

    std::cout << "OCONF \n[";
    for(int j = 0; j < N; j++)
    {
      max = 0.0;
      for(int i = 0; i < N; i++)
      {
        b_ji[j][i] = b_ji[j][i]*(1-m_ji[j][i]); // quanto menor a probabilidade, mais mantém de bij, permitindo reduzir com maior probabilidade
        if (b_ji[j][i] > max) {
          max = b_ji[j][i];
        }
      }
      for(int i = 0; i < N; i++)
      {
        b_ji[j][i] /= max;
      }
      std::cout << o_conflict[j] << "\t\t";
      if (o_conflict[j] == 1) { // somente zera quando nao se teve conflito com mais de 1!
        o_punish[j] = 1e-1;
        o_conflict[j] = 0;
      } else if (o_conflict[j] <= 0) { // quem nao foi selecionado reduz!
        if (o_conflict[j] == 0)
          o_punish[j] = -1e-1;
        else 
          o_punish[j] *= alpha; 
        o_conflict[j] -= 1;
      } else {
        o_conflict[j] = 0;
      }
      used[j] = 0;
    }
    std::cout << "]\n";

    // imprimindo valores
    {
      for(int j = 0; j < N; j++)
      {
        std::cout << "[";
        for(int i = 0; i < N; i++)
        {
          std::cout << m_ji[j][i] << "\t\t";
        }  
        std::cout << "]\n";    
      }

      std::cout << "...... BJI\n";
      for(int j = 0; j < N; j++)
      {
        std::cout << "[";
        for(int i = 0; i < N; i++)
        {
          std::cout << b_ji[j][i] << "\t\t";
        }  
        std::cout << "]\n";    
      }    

      std::cout << "...... opunish\n";
      std::cout << "[";
      for(int i = 0; i < N; i++)
      {
        std::cout << o_punish[i] << "\t\t";
      }  
      std::cout << "]\n";
    }  

    if (odd_even%2==0) {   // column
      z.clear();
      validate.clear();
      std::cout << "Z \n[";
      for(loc = 0; loc < N; loc++) // locations
      {
        z.push_back(0.0);
        validate.push_back(0.0);
        for(uav = 0; uav < N; uav++) // uavs
        {
          z[loc] += std::exp(-((b_ji[uav][loc]+o_punish[loc])/temp));          
        } 
        std::cout << z[loc] << "\t\t";
      }      
      std::cout << "]\n";
    } else { // line
      z.clear();
      validate.clear();
      std::cout << "Z \n[";
      for(uav = 0; uav < N; uav++) // uavs
      {
        z.push_back(0.0);
        validate.push_back(0.0);
        for(loc = 0; loc < N; loc++) // locations
        {
          z[uav] += std::exp(-((b_ji[uav][loc]+o_punish[loc])/temp));          
        } 
        std::cout << z[uav] << "\t\t";
      }      
      std::cout << "]\n";
    }
        
    std::cout << "...... MJI\n";
    check = 0;   
    for(uav = 0; uav < N; uav++) // uavs
    {
      max = 0.0;
      for(loc = 0; loc < N; loc++) // locations
      {
        m_ji[uav][loc] = std::exp(-((b_ji[uav][loc]+o_punish[loc])/temp)) / ((odd_even%2==0) ? z[loc] : z[uav]);
        if (m_ji[uav][loc] > max) {
          max = m_ji[uav][loc];
          p_max = loc;
        }
        if (std::isnan(m_ji[uav][loc])) {
          std::cout << "NAN!\n";
          goto out;
        }
        if (m_ji[uav][loc] == 1.0) {
          std::cout << "Fixed in 1.0! [" << uav << "]\n";
          check++;
        }
      }
      std::cout << p_max << " -> [" << o_punish[p_max] << ",";
      if (o_punish[p_max] < 0) {
        o_punish[p_max] = 1e-1;
        o_conflict[p_max] = 1;
      } else {
        o_punish[p_max] *= alpha; 
      o_conflict[p_max]++;
      }
      std::cout << o_punish[p_max] << "] ---- bij " << b_ji[uav][p_max] << "\n";
     
      if (odd_even%2==0) { // column - loc
        proposed_LOC[uav] = p_max;
      } else { // line -- uav
        proposed_UAV[uav] = p_max;
      }

      // frufru
      std::cout << "[";
      for(loc = 0; loc < N; loc++) // location
      {        
        if (loc == p_max) {
          std::cout << m_ji[uav][loc] << "*\t\t";
        } else {
          std::cout << m_ji[uav][loc] << "\t\t";
        }
      }
      std::cout << " : " << p_max << "]\n";
    }  

    cost = 0.0;
    equal = true;
    for(int i = 0; i < N; i++) // uavs
    {
      if (proposed_LOC[i] != proposed_UAV[i] || used[proposed_UAV[i]]) {
        equal = false;
        cost = -1; // not a valid solution
        break;
      } 
      cost += c_ji[i][proposed_LOC[i]];
      used[proposed_UAV[i]] = 1;
    }

    if (cost != -1) {
      std::cout << "Valid solution\n";
    }

    if (equal) {
      if (cost < cost_FINAL) {
        std::cout << "----> New final solution!\n";
        cost_FINAL = cost;
        proposed_FINAL = proposed_UAV;
      }
    }    

    // frufru
    std::cout << "LOC [";
    for(int i = 0; i < N; i++) // uavs
    {
      std::cout << proposed_LOC[i] << "\t";
    }
    std::cout << "]\n";
    std::cout << "UAV [";
    for(int i = 0; i < N; i++) // uavs
    {
      std::cout << proposed_UAV[i] << "\t";
    }
    std::cout << "]\n";

    temp *= 0.9;
    odd_even++;
  }


  out:
  std::cout << "OUT --->cost = " << cost << std::endl;
  std::vector<int> used_loc (N,0);
  double sum_used=0, sum_used_loc=0;
  if (cost == -1) {
    std::cout << "-1 cost !\n";
    // frufru
    std::cout << "LOC [";
    for(uav = 0; uav < N; uav++) // uavs
    {
      std::cout << proposed_LOC[uav] << "\t";
    }
    std::cout << "]\n";
    std::cout << "UAV [";
    for(uav = 0; uav < N; uav++) // uavs
    {
      std::cout << proposed_UAV[uav] << "\t";
    }
    std::cout << "]\n";
    long double cost_LOC = 0.0;
    long double cost_UAV = 0.0;
    for(uav = 0; uav < N; uav++) // uavs
    {
      cost_LOC += c_ji[uav][proposed_LOC[uav]] + used_loc[proposed_LOC[uav]];
      cost_UAV += c_ji[uav][proposed_UAV[uav]] + used[proposed_UAV[uav]];
      used_loc[proposed_LOC[uav]] = 1;
      sum_used_loc += used_loc[proposed_LOC[uav]];
      used[proposed_UAV[uav]] = 1;
      sum_used += used[proposed_UAV[uav]];
    }
    std::cout << "CLOC="<< cost_LOC << "\tCUAV=" << cost_UAV << std::endl;
    cost = cost_UAV;
    proposed_FINAL = proposed_UAV;
    if (cost_LOC < cost_UAV) {
      cost = cost_LOC;
      proposed_FINAL = proposed_LOC;
    }
  }

  return proposed_FINAL;
}

std::vector<std::vector<long double> > positioning (std::vector<std::vector<long double> > b_ij, int N, std::string path) {
  // normalize bij
  double max = 0;
  for (unsigned i = 0; i < N; ++i) // UAV
  {
    for (unsigned j = 0; j < N; ++j) // LOC
    {
      if (b_ij[i][j] > max) {
        max = b_ij[i][j];
      }
    }
  }
  for (unsigned i = 0; i < N; ++i) // UAV
  {
    for (unsigned j = 0; j < N; ++j) // LOC
    {
      b_ij[i][j] /= max;
    }
  }

  std::ostringstream os;
  os << path << "/bij_norm.txt";
  PrintMatrix (b_ij, os.str());

  srand(time(NULL));
  double temp = 0.9;
  // Mai
  std::vector<std::vector<long double> > m_ij;
  // variavel da tranformacao algebrica (parte do self-amplification)
  std::vector<std::vector<long double> > o_ij;
  // variavel da transformacao algebrica (parte o Y_sch)
  std::vector<std::vector<long double> > lamb_ij;
  // q_ai
  std::vector<std::vector<long double> > q_ij;
  // Ptr<UniformRandomVariable> e_ai = CreateObject<UniformRandomVariable>(); // Padrão [0,1]
  // e_ai->SetAttribute ("Min", DoubleValue (min));
  double rdom;

  for (unsigned i = 0; i < N; ++i) // UAV
  {
    m_ij.push_back(std::vector<long double>());
    o_ij.push_back(std::vector<long double>());
    lamb_ij.push_back(std::vector<long double>());
    q_ij.push_back(std::vector<long double>());
    for (unsigned j = 0; j < N; ++j) // LOC
    {
      // rdom = (double)rand()/RAND_MAX; // [0,1]
      m_ij[i].push_back(1.0/(double)N); // + rdom);
      o_ij[i].push_back(m_ij[i][j]);
      lamb_ij[i].push_back(0.0);
      q_ij[i].push_back(0.0);
    }
  }

  int print = 0;
  os.str("");
  os << path << "/I_mij.txt";
  PrintMatrix (m_ij, os.str());

  unsigned itB_max = 1000;
  unsigned itA_max = 1000;
  unsigned itC_max = 500;
  double gamma = 0.99;

  // Part A
  unsigned itA = 0;
  long double new_mij;
  bool converge_B = true, converge_C = true; // para saber se houve mudanca no valor
  while (temp >= 0.005 && itA < itA_max)
  {
    // Part B
    unsigned itB = 0;
    // do
    // {
    //   converge_B = true;
      
      for (unsigned i = 0; i < N; ++i)
      {
        for (unsigned j = 0; j < N; ++j)
        {
          // calculate \lamb_{ij}
          // NS_LOG_DEBUG ("MIJ [" << i << "," << j << "] : " << m_ij[i][j]);
          lamb_ij[i][j] = m_ij[i][j] * b_ij[i][j];
          // calculate Q_{ij}
          q_ij[i][j] =  gamma * o_ij[i][j] - lamb_ij[i][j] * b_ij[i][j];
          // calculate m_{ij}
          new_mij = expl(- (q_ij[i][j] / (long double) temp));
          if (m_ij[i][j] != new_mij) { // mudou!
            converge_B = false;
            converge_C = false;
          }
          m_ij[i][j] = new_mij;
        }
      }

      // Part C
      // unsigned itC = 0;
      // os.str("");
      // do
      // {
      //   converge_C = true;      
      //   unsigned k = 0;
      //   long double total;
      //   for (unsigned i = 0; i < N; ++i)
      //   {
      //     total = 0.0;
      //     for (k = 0; k < N; ++k)
      //     {
      //       total += m_ij[i][k];
      //     }
      //     for (k = 0; k < N; ++k)
      //     {
      //       new_mij = m_ij[i][k] / total;
      //       if (m_ij[i][k] != new_mij) {
      //         converge_B = converge_C = false;
      //       }
      //       m_ij[i][k] = new_mij;
      //     }
      //   }

      //   for (unsigned i = 0; i < N; ++i)
      //   {
      //     total = 0.0;
      //     for (k = 0; k < N; ++k)
      //     {
      //         total += m_ij[k][i];
      //     }
      //     for (k = 0; k < N; ++k)
      //     {
      //         new_mij = m_ij[k][i] / total;
      //         if (m_ij[k][i] != new_mij) {
      //             converge_B = converge_C = false;
      //         }
      //         m_ij[k][i] = new_mij;
      //     }
      //   }
      //   itC++;
      // } while (!converge_C && itC < itC_max);

    //   itB++;
    // } while (!converge_B && itB < itB_max);

    // os.str("");
    // os << path << "/A_mij_" << std::setfill ('0') << std::setw (7) << itA << ".txt";
    // PrintMatrix (m_ij, os.str());
    // os.str("");
    // os << path << "/A_lamb_ij_" << std::setfill ('0') << std::setw (7) << itA << ".txt";
    // PrintMatrix (lamb_ij, os.str());
    // os.str("");
    // os << path << "/A_q_ij_" << std::setfill ('0') << std::setw (7) << itA << ".txt";
    // PrintMatrix (q_ij, os.str());

    // long double sum = 0.0, lin=0, col=0;
    // for (unsigned i = 0; i < N; ++i)
    // {
    //   lin=0;
    //   for (unsigned j = 0; j < N; ++j)
    //   {
    //     sum += m_ij[i][j] * b_ij[i][j];
    //     lin += m_ij[i][j] * b_ij[i][j];
    //   }
    //   if (lin > 1.0 || lin < 1.0) {
    //     std::cout << "lin:" << lin << "\t";
    //   }

    //   col=0;
    //   for (unsigned k = 0; k < N; ++k)
    //   {
    //     col += m_ij[k][i] * b_ij[k][i];
    //   }
    //   if (col > 1.0 || col < 1.0) {
    //     std::cout << "col:" << col << "\t";
    //   }
    // }

    // std::cout << "\n==> itA:" << itA << "\tTemp:" << temp << "\tSum:" << sum << "\n";

    temp *= 0.99;
    o_ij = m_ij;
    itA++;
  }
  // permite sair dos lacos ao encontrar 1 para cada localizacao
  out:
  os.str("");
  os << path << "/F_mij.txt";
  PrintMatrix (m_ij, os.str());
  return m_ij;
}

std::vector<int> min_conf;
std::vector<std::vector<long double> > g_b_ij;
double glob_val = 1000.0;

void permute_uav (std::vector<int> uav_loc, int start, int end, int N) {
  std::cout << "[" << start << "," << end << "] ";
  if (start == end) {
    double val = 0.0;
    for (int i = 0; i<N; ++i) {
      std::cout << uav_loc[i] << " ";
      // val = val + g_b_ij[uav_loc[i]][i];
      val = val + g_b_ij[i][uav_loc[i]];
    }
    std::cout << std::endl;
    if (val < glob_val) {
      std::cout << "New value: " << val << std::endl;
      glob_val = val;
      for (int i = 0; i<N; ++i) { // copiando
        min_conf[i] = uav_loc[i];
      }
    }
  } else {
    for (int i = start; i <= end; ++i) {
      int aux = uav_loc[i];
      uav_loc[i] = uav_loc[start];
      uav_loc[start] = aux;
      permute_uav(uav_loc, start+1, end, N);
      // backtrack
      aux = uav_loc[i];
      uav_loc[i] = uav_loc[start];
      uav_loc[start] = aux;
    }
  }
}

void permute_loc (std::vector<int> uav_loc, int start, int end, int N) {
  std::cout << "[" << start << "," << end << "] ";
  if (start == end) {
    double val = 0.0;
    for (int i = 0; i<N; ++i) {
      std::cout << uav_loc[i] << " ";
      // val = val + g_b_ij[uav_loc[i]][i];
      val = val + g_b_ij[uav_loc[i]][i]; // variando a coluna e procurando o menor
    }
    std::cout << std::endl;
    if (val < glob_val) {
      std::cout << "New value: " << val << std::endl;
      glob_val = val;
      for (int i = 0; i<N; ++i) { // copiando
        min_conf[uav_loc[i]] = i; // salvando uav/loc para nao ser necessario converter!
      }
    }
  } else {
    for (int i = start; i <= end; ++i) {
      int aux = uav_loc[i];
      uav_loc[i] = uav_loc[start];
      uav_loc[start] = aux;
      permute_loc (uav_loc, start+1, end, N);
      // backtrack
      aux = uav_loc[i];
      uav_loc[i] = uav_loc[start];
      uav_loc[start] = aux;
    }
  }
}

std::vector<int> exhaustive (std::vector<std::vector<long double> > b_ij, int N, int op) {
  glob_val = 1000.0;
  min_conf.clear();
  for (int i = 0; i<N; ++i) {
    g_b_ij.push_back(std::vector<long double>());
    for(int j = 0; j < N; j++)
    {      
      g_b_ij[i].push_back(b_ij[i][j]);
    }
  }
  
  std::vector<int> uav_loc;
  for (int i = 0; i<N; ++i) {
    uav_loc.push_back(i);
    min_conf.push_back(i);
  }  

  switch (op) {
    case 1: // uav
      permute_uav(uav_loc, 0, N-1, N);
      break;
    case 2: // loc
      permute_loc(uav_loc, 0, N-1, N);
      break;
  }

  // std::cout << "Best: ";
  // double t = 0;
  // for (int i = 0; i < N; ++i) {
  //   std::cout << min_conf[i] << "[" << b_ij[i][min_conf[i]] << "] ";
  //   t += b_ij[i][min_conf[i]];
  // }  
  // std::cout << "---> " << t << std::endl;

  for (int i = 0; i<N; ++i) {
    g_b_ij[i].clear();
  }
  g_b_ij.clear();
  return min_conf;
}