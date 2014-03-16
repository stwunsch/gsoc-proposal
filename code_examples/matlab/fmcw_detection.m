function [ stream ] = fmcw_detection( IQ_block, radar, samp_fmcw, t_stamp )
%DETECTION FMCW Convertion of FMCW IQ-Data to Item

% Item definition
% ID : generated with tic()
% v : velocity of target
% r : range of target
% rcs : kind of RCS, more like a value for amplitude in fft
% a1 : angle1, not used
% a2 : angle2, not used
% t_stamp : timestamp

% Stream definition
% Header [N, -1, -1, -1, -1, -1, -1] with N targets detected
% N vector with [ID, v, r, rcs, a1, a2, t_stamp]

% Get parameters
c = 3e8;
f_s = radar{2};

stream = {};
a1 = 0;
a2 = 0;
rcs = 0;

s_up = samp_fmcw{1};
s_down = samp_fmcw{2};
s_cw = samp_fmcw{3};
f_d = samp_fmcw{4};
f_0 = samp_fmcw{5};

% Separate IQ-block
IQ_up = IQ_block(1:s_up);
IQ_down = IQ_block(s_up+1:s_up+s_down);
IQ_cw = IQ_block(s_up+s_down+1:end);

% Get frequency vectors
F_up = f_s*linspace(0,1,s_up);
F_down = f_s*linspace(0,1,s_down);
F_cw = f_s*linspace(0,1,s_cw);

% Calc fft
X_up = abs(fft(IQ_up,s_up)/s_up);
X_down = abs(fft(IQ_down,s_down)/s_down);
X_cw = abs(fft(IQ_cw,s_cw)/s_cw);

% Get doppler-peaks
mph = 10*mean(X_cw); % find peaks that are greater than mph
[pks,num] = findpeaks(X_cw,'MINPEAKHEIGHT',mph);
freq_cw = F_cw(num);
for k=1:size(freq_cw,2)
    if freq_cw(k)>f_s/2
        freq_cw(k)=freq_cw(k)-f_s;
    end
end
freq_cw = -freq_cw; % Why?? Dopplerpeaks are on the false side in the spectrum...

% Get up-chirp peaks
[pks,num] = findpeaks(X_up,'MINPEAKHEIGHT',mph);
freq_up = F_up(num);
for k=1:size(freq_up,2)
    if freq_up(k)>f_s/2
        freq_up(k)=freq_up(k)-f_s;
    end
end

% Get down-chirp peaks
[pks,num] = findpeaks(X_down,'MINPEAKHEIGHT',mph);
freq_down = F_down(num);
for k=1:size(freq_down,2)
    if freq_down(k)>f_s/2
        freq_down(k)=freq_down(k)-f_s;
    end
end

% Calc r-v-pairs
a = 2*f_d/c/s_up*f_s;
b = 2*f_0/c;

rv = {};
for m=1:size(freq_up,2)
    for n=1:size(freq_down,2)
        rv{m,n}(1) = (freq_up(m)-freq_down(n))/2/a;
        rv{m,n}(2) = (-freq_up(m)-freq_down(n))/2/b;
    end
end

% Match with cw-peaks
rv_match = {};
v_match = freq_cw*c/f_0/2;
for k=1:size(v_match,2)
    rv_hold = [];
    for m=1:size(freq_up,2)
        for n=1:size(freq_down,2)
            if isempty(rv_hold)
                rv_hold(1) = rv{m,n}(1);
                rv_hold(2) = rv{m,n}(2);
            elseif abs(rv{m,n}(2)-v_match(k))<abs(rv_hold(2)-v_match(k))
                rv_hold(1) = rv{m,n}(1);
                rv_hold(2) = rv{m,n}(2);
            end
        end
    end
    rv_match{k} = rv_hold;
end

% Generate item
header = {{size(rv_match,2) -1 -1 -1 -1 -1 -1}};
stream = [stream header];
for k=1:size(rv_match,2)
    ID = tic(); % get a unique ID with cpu-clock
    item = {{ID rv_match{k}(2) rv_match{k}(1) rcs a1 a2 t_stamp}};
    stream = [stream item];
end

end

