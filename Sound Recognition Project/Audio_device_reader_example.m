       devReader = audioDeviceReader;
       fileWriter = dsp.AudioFileWriter('myspeech.wav','FileFormat','WAV');
       setup(devReader);       % Initialize audio device
       disp('Speak into microphone now');
       tic;
       while toc < 10,
         [audioIn, nOverrun] = record(devReader);
         step(fileWriter, audioIn);
         if nOverrun > 0
            fprintf('Audio device reader queue was overrun by %d samples.\n',...
                     nOverrun);
         end
       end
       fprintf('Latency due to sound card''s input buffer: %f seconds.\n', ...
                devReader.SamplesPerFrame/devReader.SampleRate);
       release(devReader);     % release the audio device
       release(fileWriter);    % close the output file
       disp('Recording complete');