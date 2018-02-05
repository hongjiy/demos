function y = shift_my_claps(fs, IN_sample_claps, sample_claps, white_noise)

    
        trigger_similarity = 0.36;
        steps = 3/0.5;
        L_white_noise = fs * 0.5;
    %today I set window witdth to 3s and white noise length to 0.5s
        my_corrs = zeros(1, steps);
        temp = IN_sample_claps;
        
        for i = 1 : steps

        my_corrs(i) = corr(temp, sample_claps);
        
        if (my_corrs(i) > trigger_similarity)
            
            y = 1;
            break;
            
        end
        
        %shift signal now
        
        temp (L_white_noise + 1: end) = temp (1: end - L_white_noise);
        temp (1 : L_white_noise) = white_noise;
        
%         plot(temp)
%         pause(1);
        
        end
        
        if (my_corrs(i) < trigger_similarity)
            
            y = 0;
            
        end
        
my_corrs
        
end