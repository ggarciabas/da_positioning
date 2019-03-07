# -*- coding: UTF-8 -*-
# libraries and data
import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
import seaborn as sns
import glob
import os
import sys

run = int(sys.argv[1])
test = ''
cenarios = []
if run == 1:
    test = 'cenarios'
    f_file = open('cenarios.tex', 'w')
    cenarios = ['c2_250', 'c2_500', 'c2_1875', 'c2_3375', 'c3_125', 'c3_1000', 'c3_2750']
else:
    test = 'random'
    f_file = open('random.tex', 'w')
    cenarios = ['random_2','random_3','random_4','random_5','random_6','random_7','random_8','random_9','random_10']

global_compare = {'Exhaustive':[], 'Pos. Alg.':[], 'Random':[], 'Sequential':[]}
for cenario in  cenarios:
    print cenario
    # read solutions
    file = open(cenario+'_out.txt', 'r')
    line = file.readline().strip()
    N = int(line)
    bij = {}
    m = 0
    for i in np.arange(0,N,1):
        line = file.readline().strip()
        d = [float(x) for x in line.split(',')[:-1]]
        m = max(m, max(d))
        bij[i] = d
        # print bij[i]
    line = file.readline().strip()
    exh = [int(x) for x in line.split(',')[:-1]]
    line = file.readline().strip()
    pro = [int(x) for x in line.split(',')[:-1]]
    line = file.readline().strip()
    rnd = [int(x) for x in line.split(',')[:-1]]
    line = file.readline().strip()
    seq = [int(x) for x in line.split(',')[:-1]]
    file.close()

    # # normalize to better read the graphic
    # for k,v in bij.iteritems():
    #     for i in np.arange(0,len(v),1):
    #         v[i] = v[i] / m

    df_bij = pd.DataFrame(bij)
    cmap = sns.cubehelix_palette(50, hue=0.05, rot=0, light=0.9, dark=0, as_cmap=True)
    # Default heatmap: just a visualization of this square matrix
    sns.heatmap(df_bij, cmap=cmap)
    plt.xlabel("UAV")
    plt.ylabel(u"Location")
    plt.savefig(cenario+'_c_ij.svg')
    plt.savefig(cenario+'_c_ij.png')
    plt.savefig(cenario+'_c_ij.eps')
    plt.close()

    comp = {}
    # EXHAUSTIVE
    exh_ = np.zeros((N,N)) # [LOC,UAV]
    s = 0
    for i in np.arange(0,N,1):
        exh_[exh[i],i] = 1.0
        s = s + bij[i][exh[i]]
    comp['Exhaustive'] = s
    global_compare['Exhaustive'].append(s)
    df_exh = pd.DataFrame(exh_)
    sns.heatmap(df_exh, cmap=cmap)
    plt.title("Exhaustive")
    plt.xlabel("UAV")
    plt.ylabel(u"Location")
    plt.savefig(cenario+'_exhaustive.svg')
    plt.savefig(cenario+'_exhaustive.png')
    plt.savefig(cenario+'_exhaustive.eps')
    plt.close()

    # PROPOSED
    pro_ = np.zeros((N,N)) # [LOC,UAV]
    s = 0
    for i in np.arange(0,N,1):
        pro_[pro[i],i] = 1.0
        s = s + bij[i][pro[i]]
    comp['Pos. Alg.'] = s
    global_compare['Pos. Alg.'].append(s)
    df_pro = pd.DataFrame(pro_)
    sns.heatmap(df_pro, cmap=cmap)
    plt.title("Pos. Alg.")
    plt.xlabel("UAV")
    plt.ylabel(u"Location")
    plt.savefig(cenario+'_proposed.svg')
    plt.savefig(cenario+'_proposed.png')
    plt.savefig(cenario+'_proposed.eps')
    plt.close()

    # RANDOM
    rnd_ = np.zeros((N,N)) # [LOC,UAV]
    s = 0
    for i in np.arange(0,N,1):
        rnd_[rnd[i],i] = 1.0
        s = s + bij[i][rnd[i]]
    comp['Random'] = s
    global_compare['Random'].append(s)
    df_rnd = pd.DataFrame(rnd_)
    sns.heatmap(df_rnd, cmap=cmap)
    plt.title("Random")
    plt.xlabel("UAV")
    plt.ylabel(u"Location")
    plt.savefig(cenario+'_random.svg')
    plt.savefig(cenario+'_random.png')
    plt.savefig(cenario+'_random.eps')
    plt.close()

    # SEQUENTIAL
    seq_ = np.zeros((N,N)) # [LOC,UAV]
    s = 0
    for i in np.arange(0,N,1):
        seq_[seq[i],i] = 1.0
        s = s + bij[i][seq[i]]
    comp['Sequential'] = s
    global_compare['Sequential'].append(s)
    df_seq = pd.DataFrame(seq_)
    sns.heatmap(df_seq, cmap=cmap)
    plt.title("Sequential")
    plt.xlabel("UAV")
    plt.ylabel(u"Location")
    plt.savefig(cenario+'_sequential.svg')
    plt.savefig(cenario+'_sequential.png')
    plt.savefig(cenario+'_sequential.eps')
    plt.close()

    df_comp = pd.DataFrame(comp, index=[cenario])
    ax = df_comp.plot.bar(rot=0)
    # ax.set_title('Comparativo do custo total')
    ax.set_ylabel('Total cost')
    plt.tight_layout()
    plt.savefig(cenario+'_compare.svg')
    plt.savefig(cenario+'_compare.png')
    plt.savefig(cenario+'_compare.eps')
    plt.close()

    # SLIDE
    f_file.write("""\n\\begin{frame}{"""+cenario+' - Cost'+"""}
        \\begin{figure}[!htb]
            \\includegraphics[width=\\textwidth]{"""+cenario+'_c_ij.eps'+"""}
        \\end{figure}
    \\end{frame}""")
    f_file.write("""\n\\begin{frame}{"""+cenario+' - Mij'+"""}
    \\begin{columns}
        \\begin{column}{0.45\\textwidth}
            \\begin{figure}[!htb]
                    \\includegraphics[width=\\textwidth]{"""+cenario+'_exhaustive.eps'+"""}
                \\end{figure}
                \\vspace{-0.5cm}
            \\begin{figure}[!htb]
                    \\includegraphics[width=\\textwidth]{"""+cenario+'_proposed.eps'+"""}
                \\end{figure}
            \\end{column}
            \\begin{column}{0.45\\textwidth}
            \\begin{figure}[!htb]
                    \\includegraphics[width=\\textwidth]{"""+cenario+'_random.eps'+"""}
                \\end{figure}
                \\vspace{-0.5cm}
            \\begin{figure}[!htb]
                    \\includegraphics[width=\\textwidth]{"""+cenario+'_sequential.eps'+"""}
                \\end{figure}
            \\end{column}
        \\end{columns}
    \\end{frame}""")
    f_file.write("""\n\\begin{frame}{"""+cenario+' - Compare'+"""}
        \\begin{figure}[!htb]
            \\includegraphics[width=\\textwidth]{"""+cenario+'_compare.eps'+"""}
        \\end{figure}
    \\end{frame}""")

df_global = pd.DataFrame(global_compare, index=cenarios)
ax = df_global.plot.bar(rot=45)
ax.set_ylabel('Total cost')
ax.set_xlabel('Scenarios')
plt.tight_layout()
plt.savefig(test+'_global_compare.svg')
plt.savefig(test+'_global_compare.png')
plt.savefig(test+'_global_compare.eps')
plt.close()

# SLIDE
f_file.write("""\n\\begin{frame}{"""+cenario+' - Global Compare'+"""}
    \\begin{figure}[!htb]
        \\includegraphics[width=\\textwidth]{"""+test+'_global_compare.eps'+"""}
    \\end{figure}
\\end{frame}""")

f_file.close()