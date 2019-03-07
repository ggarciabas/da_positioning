g++ -o main main.cc

./main < random_2.txt > random_2_log.txt
./main < random_3.txt > random_3_log.txt
./main < random_4.txt > random_4_log.txt
./main < random_5.txt > random_5_log.txt
./main < random_6.txt > random_6_log.txt
./main < random_7.txt > random_7_log.txt
./main < random_8.txt > random_8_log.txt
./main < random_9.txt > random_9_log.txt
./main < random_10.txt > random_10_log.txt

python plot_compare.py 0

./main < c2_250.txt > c2_250_log.txt
./main < c2_500.txt > c2_500_log.txt
./main < c2_1875.txt > c2_1875_log.txt
./main < c2_3375.txt > c2_3375_log.txt
./main < c3_125.txt > c3_125_log.txt
./main < c3_1000.txt > c3_1000_log.txt
./main < c3_2750.txt > c3_2750_log.txt

python plot_compare.py 1