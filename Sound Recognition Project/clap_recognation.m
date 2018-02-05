function clap_recognation()

ts = 5;
fs = 200000;
L = fs * ts;
interval = 0.1;
L_sample = interval * fs;
win = zeros(1,L);
t = 1:1:L;
init = 1;

ADR = audioDeviceReader(fs, L_sample, 'NumChannels', 1);

for i = 1: ts/interval
    
    [audioIn, nOverrun] = step(ADR);
    
    if(nOverrun > 0) 
        break;
    end
    
    for j = 1 : L_sample
        
        win( end - i * L_sample + j) = audioIn(j);
        
    end
    

end
end