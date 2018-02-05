function y = spectrum_recognition(win, ts, fs, sample_claps_P1)


L = ts*fs;
BW_reatio = 50;

P2 = abs(fft(win')/L);
P1 = P2(1:L/BW_reatio + 1);
P1(2:end - 1) = 2*P1(2:end - 1);
y = corr(P1, sample_claps_P1);


end