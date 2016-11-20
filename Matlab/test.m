A = csvread('study.csv');
B = A(:,1:1);
C = A(:,2:3);
bipl_sdnn_train('test.txt',C',B','nonaka.bin');
D = bipl_sdnn_test('nonaka.bin',C');
