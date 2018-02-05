tic;
win(L_sample:end) = win(1: end - L_sample + 1);

win(1:L_sample) = audioIn(1:end);

plot(win);

fprintf('Time shifting take %d seconds.\n',...
                     toc);