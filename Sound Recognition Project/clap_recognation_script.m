disp('Sound recognition script initiating')

if (~exist('a', 'var'))
    
    a = arduino('com5','uno');
    Suppress_waring();
    load('C:\Users\Hongji\Documents\MATLAB\Loading_Data_CP.mat');
    
end
    
ts = 3;
fs = 200000;
L = fs * ts;
interval = 0.5;

L_sample = interval * fs;
win = zeros(1,L);
t = 1:1:L;
init = 1;
counter = 0;
exit = 0;
audioIn = 0;
nOverrun = 0;

%*******************************************************************************************
%                                    Global Variables
%*******************************************************************************************

global Loop_Switch_Pin LED_Switch_Pin;


Loop_Switch_Pin = 'D10';
LED_Switch_Pin = 'D3';


 
writeDigitalPin(a, 'D6', 0);
writeDigitalPin(a, LED_Switch_Pin, 0);
PWM_Pushbottom_D11_Switch(a, 0);
writeDigitalPin(a, 'D5', 1);

%[FLAC_Y,FLAC_Fs] = audioread('C:\Users\Hongji\Desktop\Dirty Work MV.flac');
%[FLAC_Y,FLAC_Fs] = audioread('C:\Users\Hongji\Desktop\Sample.flac');
% [FLAC_Y,FLAC_Fs] = audioread('C:\Users\Hongji\Desktop\Send My Love.flac');
% player = audioplayer(FLAC_Y,FLAC_Fs);

ADR = audioDeviceReader(fs, L_sample, 'NumChannels', 1);
%ADR = dsp.AudioRecorder(fs, L_sample, 'NumChannels', 1);

% for i = 1: ts/interval
%     
%     [audioIn, nOverrun] = step(ADR);
%     
%     if(nOverrun > 0) 
%         break;
%     end
%     
%     for j = 1 : L_sample
%         
%         win( (i -1)* L_sample + j) = audioIn(j);
%         
%     end
%     
% 
% end

% % tic;

% % i = 1;

disp('Sound recognition loop starts')

while(1)
    
while(readDigitalPin(a,'D7'))
    
%       tic;
%       plot(win);
%       axis([0, 6e5, -1.2,1.2]);
     
    
    [audioIn, nOverrun] = step(ADR);
    
    if(nOverrun > 0) 
        break;
    end
    
% % % % % % % %     %time shift happens here
% % % % % % % %     for i = 1 : (L - L_sample - 1)
% % % % % % % %     
% % % % % % % %         win(L - i) = win(L - (i + L_sample));
% % % % % % % %     
% % % % % % % %     end
% % % % % % % %     
% % % % % % % %     %inster sampels here
% % % % % % % %     for i = 1 : L_sample
% % % % % % % % 
% % % % % % % %         win(i) = audioIn(i);
% % % % % % % % 
% % % % % % % %     end

win(1:end - L_sample + 1) = win(L_sample: end);

win(end - L_sample + 1: end) = audioIn;

% % % % fprintf('Audio device reader queue was overrun by %d samples.\n',...
% % % %                      nOverrun);

% % % % fprintf('Audio device reader latency is %d .\n',...
% % % %                     toc - 0.5);

                
% % % % % % % % % % % %                 massive_matrix(:,i) = audioIn;
% % % % % % % % % % % %                 i = i + 1;

%    pause(interval);   


%    s = 1
    
%     drawnow;




  
        
        %LED_control(time_domain_xcorr(win, sample_claps), a, spectrum_recognition(win, ts, fs, sample_claps_P1));
        %if (music_control(time_domain_xcorr(win, sample_claps), a, spectrum_recognition(win, ts, fs, sample_claps_P1), player))
        
        clr = music_control(time_domain_xcorr(win, sample_claps), a, spectrum_recognition(win, ts, fs, sample_claps_P1));
        if(clr)
            
            win = zeros(size(win));
            %disp('Clearing executed');
            
        end
        
        
        
        exit = ( readVoltage(a, 'A4') > 3.3 );
        
        
        if( exit )
            
            break;
            
        end


end

if(nOverrun)

        fprintf('Audio device reader queue was overrun by %d samples.\n',...
                     nOverrun);
                 
end
                 
        if( exit )
            
            break;
            
        end
        



end

disp('Sound recognition loop exited')
writeDigitalPin(a, 'D5', 0);