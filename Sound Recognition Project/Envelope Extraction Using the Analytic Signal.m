t = 0:1e-4:1;
x = [1+cos(2*pi*50*t)].*cos(2*pi*1000*t);

plot(t,x)
xlim([0 0.1])
xlabel('Seconds')

y = hilbert(x);
env = abs(y);

plot(t,x)
hold on
plot(t,[-1;1]*env,'r','LineWidth',2)
xlim([0 0.1])
xlabel('Seconds')