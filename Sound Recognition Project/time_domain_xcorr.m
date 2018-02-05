function y = time_domain_xcorr(temp, sample_claps)
%tic;
    down_temp = downsample(temp, 100);
    down_sample_claps = downsample(sample_claps, 100);
   
    XCORR = xcorr(down_temp, down_sample_claps);
    env = abs(hilbert(XCORR));
    
%     plot(XCORR)
    
    [~,locs_Rwave] = findpeaks(env,'MinPeakHeight',1,'MinPeakDistance',150);
    [~ ,npks] = size(locs_Rwave);
    
    
    
%     if(npks > 3 || npks == 3)
%         
%         y = 1;
%         
%     else
%         
%         y = 0;
%         
%     end

%toc
    y = npks;
    
end
    