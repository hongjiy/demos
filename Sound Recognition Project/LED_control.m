function LED_control(npks, a, spec_simil_ratio)

triger_slope = 3;


persistent pas_npsk flag 

if isempty(pas_npsk) 
    
    pas_npsk = 0;
    flag = 0;
    
end

if(spec_simil_ratio > 0.25)
    
    if ( (( npks - pas_npsk ) > triger_slope || ( npks - pas_npsk )== triger_slope || ( npks >2 && pas_npsk == 2 )) && flag == 0 )

        writeDigitalPin(a, 'D6', 1);
        flag = 1;

    elseif ((( npks - pas_npsk ) > triger_slope || ( npks - pas_npsk )== triger_slope || ( npks >2 && pas_npsk == 2 )) && flag == 1)

        writeDigitalPin(a, 'D6', 0);
        flag = 0;

    end


%     if (  (npks == pas_npsk && npks > 2 ) && flag == 0 )
% 
%         writeDigitalPin(a, 'D6', 1);
%         flag = 1;
% 
%     elseif ((( npks - pas_npsk ) > triger_slope || ( npks - pas_npsk )== triger_slope || ( npks ==4 && pas_npsk == 2 )) && flag == 1)
% 
%         writeDigitalPin(a, 'D6', 0);
%         flag = 0;
% 
%     end
%     
end

     pas_npsk = npks
     spec_simil_ratio

end