function [ stream ] = tracking( stream )
%TRACKING Tracking of targets
%   Predefined items! Definition in detection-block

% Simple tracking:
% If the target doesn't leave a predefined range from item to item, match
% ID.

var_v = 2; % max. variation of velocity [m/s]
var_r = 2; % max. variation of rnage [m/s]

last_items = {};
N_stream = size(stream,2);
k = 1;
while k<=N_stream
    N_items = stream{k}{1}; % number of targets after header
    for m=1:N_items % get targets after header
        match_ID = -1;
        item = stream{k+m};
        if ~isempty(last_items)
            % TRACKING HERE
            
            % Compare velocity
            only_one = 0;
            for n=1:size(last_items,2)
                if (abs(last_items{n}{2}-item{2})<var_v)&&(only_one==0)
                    match_ID = last_items{n}{1};
                    only_one = 1;
                elseif (abs(last_items{n}{2}-item{2})<var_v)&&(only_one==1)
                    match_ID = -1;
                    break
                end
            end
            
            % Compare range
            item_found = 0;
            for n=1:size(last_items,2)
                if (abs(last_items{n}{3}-item{3})<var_r)&&(last_items{n}{1}==match_ID)
                    item_found = 1;
                end
            end
            if ~item_found
                match_ID = -1;
            end
            
            % set new ID if item found
            if match_ID~=-1
                stream{k+m}{1} = match_ID;
            end
            
            % END TRACKING
        end
    end
    last_items = {}; % clear last items and save current ones
    for m=1:N_items
        last_items{m} = stream{k+m};
    end
    
    k = k+N_items+1; % set counter on next header
end

end

