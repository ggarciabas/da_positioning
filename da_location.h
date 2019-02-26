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

std::vector<int> da_positioning (std::vector<std::vector<long double> > c_ji, int N) { //, std::string path) {
  std::vector<std::vector<long double> > b_ji;
  std::vector<std::vector<long double> > m_ji;
  std::vector<long double> o_loc; // ocupada
  std::vector<long double> o_uav; // ocupada
  std::vector<int> o_conflict;
  std::vector<int> proposed_FINAL;
  std::vector<int> proposed_LOC;
  std::vector<int> proposed_UAV;

  for(int j = 0; j < N; j++) // uav
  {
    m_ji.push_back(std::vector<long double>());
    b_ji.push_back(std::vector<long double>());
    for(int i = 0; i < N; i++) // location
    {
      m_ji[j].push_back(0.01);// para nao dar conflito com a primeira atualizaçao de bij
      b_ji[j].push_back(c_ji[j][i]);
    }
    o_loc.push_back(1e-1);
    o_uav.push_back(1e-1);
    proposed_LOC.push_back(-1);
    proposed_UAV.push_back(-1);
    proposed_FINAL.push_back(-1);
    o_conflict.push_back(0); // para saber se ja teve conflito 
  }

  long double temp = 0.3, t_min=9e-5, validate, alpha, max, cost, z, cost_FINAL = 5.0; // alpha punicao de capacidade
  int check, p_max;

  // std::cout << std::setprecision(10) << std::setw(10) << std::setfill(' ') << std::fixed;//scientific
  std::cout << "============================> STARTING\n";
  int odd_even = 0;
  alpha = 1.5; // aumenta 20%
  unsigned uav, loc;
  bool equal;

  // std::ofstream file;
  // std::ostringstream n_path;
  // n_path << path << "_b_graphic.txt";
  // file.open(n_path.str().c_str(), std::ofstream::out);
  // file << N << std::endl;
  while (temp > t_min) {
    // alpha = 1-temp;
    std::cout << "############################################################################### TEMP = " << temp << "  ALPHA = " << alpha << " ODDEVEN = " << odd_even << std::endl;   

    if (odd_even%2==0) {      
      // normalizando bji por linhas
      std::cout << "(1-MJI)-----\n";
       // normalizando MIJ, para suavizar a punicao de bji
      for (uav = 0; uav < N; ++uav) // UAV
      {
        max = 0.0;
        for (loc = 0; loc < N; ++loc) // LOC
        {
          if (m_ji[uav][loc] > max) {
            max = m_ji[uav][loc];
          }
        }
        for (loc = 0; loc < N; ++loc) // LOC
        {
          m_ji[uav][loc] /= max;
        }
      }
      for (uav = 0; uav < N; ++uav) // UAV
      {
        std::cout << "[";
        max = 0.0;
        for (loc = 0; loc < N; ++loc) // LOC
        {
          b_ji[uav][loc] = b_ji[uav][loc]*m_ji[uav][loc]; // quanto menor a probabilidade, mais mantém de bij, permitindo reduzir o com maior probabilidade
          std::cout << m_ji[uav][loc] << "\t\t\t\t";
          // if (b_ji[uav][loc] > max) {
          //   max = b_ji[uav][loc];
          // }
        }
        std::cout << "]\n";
        // for (loc = 0; loc < N; ++loc) // LOC
        // {
        //   b_ji[uav][loc] /= max;
        // }
        o_conflict[uav] = 0; // reiniciando conflito
      }

      // imprimindo valores
      {
        std::cout << "...... BJI\n";
        for(int j = 0; j < N; j++)
        {
          std::cout << "[";
          for(int i = 0; i < N; i++)
          {
            std::cout << b_ji[j][i] << "\t\t\t\t";
          }  
          std::cout << "]\n";    
        }    

        std::cout << "...... O_LOC\n";
        std::cout << "[";
        for(int i = 0; i < N; i++)
        {
          std::cout << o_loc[i] << "\t\t\t\t";
        }  
        std::cout << "]\n";   

        std::cout << "...... O_UAV\n";
        std::cout << "[";
        for(int i = 0; i < N; i++)
        {
          std::cout << o_uav[i] << "\t\t\t\t";
        }  
        std::cout << "]\n";
      }

      std::cout << "...... MJI -- por linha\n";
      check = 0;   
      for(uav = 0; uav < N; uav++) // uavs
      {        
        // std::cout << "Z=";
        z = 0.0;        
        for(loc = 0; loc < N; loc++) // locations
        {        
          z += std::exp(-((b_ji[uav][loc]+o_loc[loc])/temp));
        } 
        // std::cout << z << "\texp(0) = " << std::exp(0);

        max = 0.0;
        validate = 0.0;
        // std::cout << "\n(";
        // for(loc = 0; loc < N; loc++) // locations
        // {
        //   std::cout << ((b_ji[uav][loc]+o_loc[loc])/temp) << "\t\t\t\t";
        // }
        // std::cout << ")\n(";
        for(loc = 0; loc < N; loc++) // locations
        {
          m_ji[uav][loc] = std::exp(-((b_ji[uav][loc]+o_loc[loc])/temp)) / z;
          // std::cout << std::exp(-((b_ji[uav][loc]+o_loc[loc])/temp)) << "\t\t\t\t";
          // m_ji[uav][loc] = std::exp(-((b_ji[uav][loc]+o_loc[loc])/temp)) / z;
          validate += m_ji[uav][loc];
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
            proposed_UAV[uav] = loc; // proposed é UAV/loc representa para qual loc o UAv irá
          }
        }
        // std::cout << ")\n";
        o_loc[p_max] *= alpha; 
        o_conflict[p_max]++;
        proposed_UAV[uav] = p_max;
        // frufru
        std::cout << "[";
        for(loc = 0; loc < N; loc++) // location
        {        
          if (loc == p_max) {
            std::cout << m_ji[uav][loc] << "*\t\t\t\t";
          } else {
            std::cout << m_ji[uav][loc] << "\t\t\t\t";
          }
        }
        std::cout << " = " << validate << " : " << p_max << "]\n";
      }  

      // for (uav = 0; uav < N; ++uav) // UAV
      // {
      //   if (o_conflict[uav] == 1) { // somente um uav foi selecionado para a uav
      //     o_uav[uav] *= (2-alpha);
      //   } else if (o_conflict[uav] == 0) {
      //     o_uav[uav] = 0.1;
      //   }
      // }   

    } else {
      // normalizando mij para suavizar a punicao de bji
      for (loc = 0; loc < N; ++loc) // LOC
      {
        max = 0.0;
        for (uav = 0; uav < N; ++uav) // UAV
        {
          if (m_ji[uav][loc] > max) {
            max = m_ji[uav][loc];
          }
        }
        for (uav = 0; uav < N; ++uav) // UAV
        {
          m_ji[uav][loc] /= max;
        }
      }
      // normalizando bji por coluna
      for (loc = 0; loc < N; ++loc) // LOC
      {
        max = 0.0;
        for (uav = 0; uav < N; ++uav) // UAV
        {
          b_ji[uav][loc] = b_ji[uav][loc]*m_ji[uav][loc]; // quanto menor a probabilidade, mais mantém de bij, permitindo reduzir o com maior probabilidade
          // if (b_ji[uav][loc] > max) {
          //   max = b_ji[uav][loc];
          // }
        }
        // for (uav = 0; uav < N; ++uav) // UAV
        // {
        //   b_ji[uav][loc] /= max;
        //   // file << b_ji[uav]/[loc] << " ";
        // }
        o_conflict[loc] = 0; // reiniciando conflito
      }

      // imprimindo valores
      {
        std::cout << "...... (1-MJI)\n";
        for(uav = 0; uav < N; uav++)
        {
          std::cout << "[";
          for(loc = 0; loc< N; loc++)
          {
            std::cout << m_ji[uav][loc] << "\t\t\t\t";
          }  
          std::cout << "]\n";    
        } 

        std::cout << "...... BJI - por coluna\n";
        for(uav = 0; uav < N; uav++)
        {
          std::cout << "[";
          for(loc = 0; loc< N; loc++)
          {
            std::cout << b_ji[uav][loc] << "\t\t\t\t";
          }  
          std::cout << "]\n";    
        }    

        std::cout << "...... O_UAV\n";
        std::cout << "[";
        for(int i = 0; i < N; i++)
        {
          std::cout << o_uav[i] << "\t\t\t\t";
        }  
        std::cout << "]\n";  

        std::cout << "...... O_LOC\n";
        std::cout << "[";
        for(int i = 0; i < N; i++)
        {
          std::cout << o_loc[i] << "\t\t\t\t";
        }  
        std::cout << "]\n";  
      }

      std::cout << "...... MJI -- por coluna\n";
      check = 0;   
      for(loc = 0; loc < N; loc++) // locations
      {        
        // std::cout << "Z=";
        z = 0.0;
        for(uav = 0; uav < N; uav++) // uavs
        {        
          z += std::exp(-((b_ji[uav][loc]+o_uav[uav])/temp)); 
        }
        // std::cout << z << "\t";

        max = 0.0;
        validate = 0.0;
        // frufru
        // std::cout << "\n(";
        // for(uav = 0; uav < N; uav++) // uavs
        // {
        //   std::cout << ((b_ji[uav][loc]+o_uav[uav])/temp) << "\t\t\t\t";
        // }
        // std::cout << ")\n(";
        for(uav = 0; uav < N; uav++) // uavs
        {
          m_ji[uav][loc] = std::exp(-((b_ji[uav][loc]+o_uav[uav])/temp)) / z;
          // std::cout << std::exp(-((b_ji[uav][loc]+o_uav[uav])/temp)) << "\t\t\t\t";
          validate += m_ji[uav][loc];
          if (m_ji[uav][loc] > max) {
            max = m_ji[uav][loc];
            p_max = uav;
          }
          if (std::isnan(m_ji[uav][loc])) {
            std::cout << "NAN!\n";
            // file.close();
            goto out;
          }
          if (m_ji[uav][loc] == 1.0) {
            std::cout << "Fixed in 1.0! [" << loc << "]\n";
            check++;
            proposed_LOC[p_max] = loc; // proposed é UAV/loc representa para qual loc o UAv irá
          }
        }
        // std::cout << ")\n";
        o_uav[p_max] *= alpha; 
        o_conflict[p_max]++;
        proposed_LOC[p_max] = loc; // solucao final proposed é UAV/LOC
        // frufru
        std::cout << "[";
        for(uav = 0; uav < N; uav++) // uavs
        {        
          if (uav == p_max) {
            std::cout << m_ji[uav][loc] << "*\t\t\t\t";
          } else {
            std::cout << m_ji[uav][loc] << "\t\t\t\t";
          }
        }
        std::cout << " = " << validate << " : " << p_max << "]\n";
      }

      // frufru   
      std::cout << "--- mji uav/loc\n";   
      for(uav = 0; uav < N; uav++) // uavs
      {      
        std::cout << "[";
        for (loc = 0; loc < N; ++loc) // LOC
        {  
          std::cout << m_ji[uav][loc] << "\t\t\t\t";
        }
        std::cout << "]\n";
      } 

    }  

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

    cost = 0.0;
    equal = true;
    for(uav = 0; uav < N; uav++) // uavs
    {
      if (proposed_LOC[uav] != proposed_UAV[uav]) {
        equal = false;
        cost = -1; // not a valid solution
        break;
      }
      cost += c_ji[uav][proposed_LOC[uav]];
    }

    std::cout << "CUSTO---------------------------------> " << cost << "\n";

    if (equal) {
      if (cost < cost_FINAL) {
        std::cout << "----> NEw final solution!\n";
        cost_FINAL = cost;
        proposed_FINAL = proposed_UAV;
      }
    }    
      
    temp *= 0.9;
    odd_even++;
    // file << std::endl;
  }
  out:
  std::cout << "OUT --->cost = " << cost << std::endl;
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
      cost_LOC += c_ji[uav][proposed_LOC[uav]];
      cost_UAV += c_ji[uav][proposed_UAV[uav]];
    }
    std::cout << "CLOC="<< cost_LOC << "\tCUAV=" << cost_UAV << std::endl;
    cost = cost_UAV;
    proposed_FINAL = proposed_UAV;
    if (cost_LOC < cost_UAV) {
      cost = cost_LOC;
      proposed_FINAL = proposed_LOC;
    }
  }

  // std::ostringstream os;
  // os.str("");
  // os << path << "_F_mij.txt";
  // PrintMatrix (m_ji, os.str());

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
double glob_val = 0;

void permute (std::vector<int> uav_loc, int start, int end, int N) {
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
      permute(uav_loc, start+1, end, N);
      // backtrack
      aux = uav_loc[i];
      uav_loc[i] = uav_loc[start];
      uav_loc[start] = aux;
    }
  }
}

std::vector<int> exhaustive (std::vector<std::vector<long double> > b_ij, int N) {
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

  permute(uav_loc, 0, N-1, N);

  // std::cout << "Best: ";
  // double t = 0;
  // for (int i = 0; i < N; ++i) {
  //   std::cout << min_conf[i] << "[" << b_ij[i][min_conf[i]] << "] ";
  //   t += b_ij[i][min_conf[i]];
  // }  
  // std::cout << "---> " << t << std::endl;
  return min_conf;
}