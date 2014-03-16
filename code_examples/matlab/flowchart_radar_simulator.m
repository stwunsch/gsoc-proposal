%% FLOWCHART
% FLOWCHART FOR RADAR SIMULATION, TRACKING AND CLASSIFICATION
%%

clear; clc;
TSTART = tic();
TLAST = 0;

%% PLOTS AND VERBOSE

% Plot trajectories
plot_traj = 3;
% 0 : no plot
% 1 : radial range and velocity
% 2 : cartesian trajectory
% >2 : plot all

% Plot trajectories from stream-items after tracking
plot_track = 3;
% 0 : no plot
% 1 : radial range of tracked targets
% 2 : radial velocity of tracked targets
% >2 : plot all

%% PARAMETER

% Radar
c = 3e8;
t_m = 5; % measurement time [s]
f_s = 125e3; % sample frequency [Hz]
p_t = 1; % transmission power
g_tr = 1e4; % antenna gain

radar = {t_m f_s p_t g_tr};

% Modulation
f_mod = 'fmcw'; % set frequency modulation

%% TARGETS/TRAJECTORIES
% 2D-trajectories for each target
% radar is ajusted to (0,1)
% velocity is calculated with measurement time and position space

traj_steps = f_s*t_m; % stepping of the trajectories
t_v = linspace(0,t_m,traj_steps); % timevector % WARNING: Abstand der Einträge weicht fon 1/f_s ab!

% RCS
rcs = {1e3 1e3 1e3}; % ADD DATA HERE FOR NEW TRAJECTORY - Each element represents the property of one target

% Position
r1 = [linspace(-20,20,traj_steps);... % x
    linspace(40,10,traj_steps)]; % y
r2 = [linspace(0,0,traj_steps);... % x
    linspace(70,10,traj_steps)]; % y
r3 = [linspace(-20,20,traj_steps);... % x
    linspace(10,60,traj_steps)]; % y

r_xy = {r1 r2 r3}; % ADD DATA HERE FOR NEW TRAJECTORY - Each element refers to one target

% Radial range (Conversion from carthesian coordinats to Range)
r = {};
for k=1:size(r_xy,2)
    r{k} = zeros(1,size(r_xy{k},2));
    for m=1:size(r{k},2)
        r{k}(m) = sqrt( (r_xy{k}(1,m))^2 + (r_xy{k}(2,m))^2 );
    end
end

% Radial velocity (Conversion from carthesian coordinates)
v = {};
for k=1:size(r_xy,2)
    v{k} = zeros(size(r{k}));
    for m=2:size(v{k},2)
        v{k}(m) = ( r{k}(m-1)-r{k}(m) )/t_m*traj_steps;
    end
    v{k}(1) = v{k}(2);
end

% Plot trajectories
if plot_traj>0
    % radial range and velocity
    figure;
    hold on
    for k = 1:size(r,2)
        plot(t_v,r{k},'r',t_v,v{k},'b--')
    end
    hold off
    title('Radial range and velocity')
    xlabel('t / s')
    % FIXME: Add legends / dual-axis plot / two distinguished plots
end
if plot_traj>0
    % trajectories in cartesian space
    figure;
    hold on
    for k = 1:size(r,2)
        plot(r_xy{k}(1,:),r_xy{k}(2,:))
    end
    hold off
    title(sprintf('cartesian trajectory\nradar adjusted to y-axis'))
    xlabel('x / m')
    xlabel('y / m')
    % FIXME: Add legend
end

%% CREATE IQ-DATA
% based on waveform, including free-space path loss

switch f_mod
case 'fmcw'
% waveform fmcw /\_
% up-chirp f_0 -> f_0+f_d
% down-chirp f_0 + f_d -> f_0
% cw f_0 -> f_0
f_0 = 24.0e9; % base frequency [Hz]
f_d = 250e6; % sweep bandwidth [Hz]
s_up = 2048; % samples up-chirp
s_down = s_up; % samples down-chirp
s_cw = 4096; % samples cw
t_up = s_up/f_s; % duration up-chirp
t_down = s_down/f_s; % duration down-chirp
t_cw= s_cw/f_s; % duration cw
t_tot = t_up+t_down+t_cw;

% Create waveform
n_rep = ceil(t_m/t_tot); % periods of wv in measurement duration % WARNING: Last period is actually only partially in duration

t_ns_pnt =  zeros(1,n_rep*3); % nonshifted waveform (transmission) - time instants of all edges of waveform
wv_ns_pnt = zeros(1,n_rep*3); % nonshifted waveform (transmission) - frequencies at the edges of waveform
for K=0:n_rep-1
    k = 3*K+1;
    
    t_ns_pnt(k) = K*t_tot;
    t_ns_pnt(k+1) = K*t_tot+t_up;
    t_ns_pnt(k+2) = K*t_tot+t_up+t_down;
    
    wv_ns_pnt(k) = f_0;
    wv_ns_pnt(k+1) = f_0+f_d;
    wv_ns_pnt(k+2) = f_0;
end
wv_ns = interp1(t_ns_pnt,wv_ns_pnt,t_v); % interpolierte Frequenzkurve über ges. Messdauer

for k=1:size(r,2) % create a waveform for each target (time-shift and doppler-shift)
    t_s_pnt = zeros(size(wv_ns)); % length of received waveform = lenght of transmitted waveform
    wv_s_pnt = zeros(size(wv_ns)); % length of received waveform = lenght of transmitted waveform
    for m=1:size(wv_ns,2)
        t_s_pnt(m) = t_v(m)+2*r{k}(m)/c; % shift time-points
        wv_s_pnt(m) = wv_ns(m)+2*v{k}(m)*f_0/c; % shift nonshifted waveform, velocity is not shifted in tau!
    end
    wv_s{k} = interp1(t_s_pnt,wv_s_pnt,t_v);
    for m=1:size(wv_s{k},2) % delete NaN values
        if isnan(wv_s{k}(m))
            wv_s{k}(m) = f_0;
        end
    end
end

end % end switch

% Integrate transmitted signal
t_step = t_m/size(wv_ns,2);
phase_t = 0;
s_t = zeros(size(wv_ns));
for k=1:size(wv_ns,2)
    s_t(k) = sqrt(p_t)*exp(1i*2*pi*phase_t);
    phase_t = phase_t + wv_ns(k)*t_step;
end

% Integrate received signal
% signal-power adjusted with free-space path loss (radar-equation)
s_r = zeros(size(wv_ns));
for m = 1:size(wv_s,2)
    phase_r = 0;
    for k=1:size(wv_s{m},2)
        p_r = p_t*g_tr^2*c^2*rcs{m}/(4*pi)^3/(r{m}(k))^4/f_0^2;
        s_r(k) = s_r(k) + sqrt(p_r)*exp(1i*2*pi*phase_r);
        phase_r = phase_r + wv_s{m}(k)*t_step;
    end
end

% Calc IQ-Data
IQ = 0.5*(real(s_t.*conj(s_r))+1i*imag(s_t.*conj(s_r)));

TIQ = toc(TSTART)-TLAST;
TLAST = toc(TSTART);

%% DETECTION
% Calculation of range and velocity respectivly to frequency-modulation
% Item is defined as vector [ID velocity range rcs angle1 angle2]

switch f_mod
case 'fmcw'

s_tot = s_up+s_down+s_cw; % samples for one modulation cycle
n_det = floor(t_m/t_tot); % number of full blocks
samp_fmcw = {s_up s_down s_cw f_d f_0}; % data structure to represent fmcw waveform

stream = {};

for m=1:n_det % go through blocks
    IQ_block = IQ(s_tot*(m-1)+1:s_tot*m); % crop blockwise
    t_stamp = (m-1)*t_tot; % timestamp for detected peaks (current element in stream: (m-1))
    stream = [stream fmcw_detection(IQ_block, radar, samp_fmcw, t_stamp)]; % structure which holds all detected objects consecutively
end

end % end switch

TDETEC = toc(TSTART)-TLAST;
TLAST = toc(TSTART);

%% TRACKING

stream = tracking(stream); % Correction of target IDs

% Get tracked targets
targ_track = {};
for k=1:size(stream,2)
    if stream{k}{7} == -1 % if item is an header
        continue % skip item
    end
    if isempty(targ_track) % initialize
        targ_track{1}{1} = stream{k};
        continue
    end
    in_list = 0;
    for m=1:size(targ_track,2)
        if stream{k}{1} == targ_track{m}{1}{1} % if ID found, store item
            targ_track{m}{end+1} = stream{k};
            in_list = 1;
            break
        end
    end
    if ~in_list % if not in list, create new item in targetlist
        targ_track{end+1}{1} = stream{k};
    end
end

% Get velocity, range and time vectors for tracked items
r_track = {};
v_track = {};
t_track = {};
for k=1:size(targ_track,2)
    for m=1:size(targ_track{k},2)
        t_track{k}(m) = targ_track{k}{m}{7};
        r_track{k}(m) = targ_track{k}{m}{3};
        v_track{k}(m) = targ_track{k}{m}{2};
    end
end

% Plot tracked targets
if plot_track>0
    % range
    figure;
    hold all
    for k=1:size(targ_track,2)
        plot(t_track{k},r_track{k})
    end
    hold off
    title('Tracked targets: Range')
    xlabel('t / s')
    ylabel('r / m')
end

if plot_track>0
    % velocity
    figure;
    hold all
    for k=1:size(targ_track,2)
        plot(t_track{k},v_track{k})
    end
    hold off
    title('Tracked targets: Velocity')
    xlabel('t / s')
    ylabel('v / m/s')
end

TTRACK = toc(TSTART)-TLAST;
TLAST = toc(TSTART);

%% CLASSIFICATION

TCLASS = toc(TSTART)-TLAST;
TLAST = toc(TSTART);

%% OUTPUT

disp('// RADAR SIMULATION, TRACKING AND CLASSIFICATION')

disp('TARGETS')
fprintf('\tNumber: %g\n', size(r,2));

disp('RUNTIME')
fprintf('\tCreate IQ-Data: %gs\n', TIQ)
fprintf('\tDetection: %gs\n', TDETEC)
fprintf('\tTracking: %gs\n', TTRACK)
fprintf('\tClassification: %gs\n', TCLASS)
fprintf('\tFlowchart: %gs\n', toc(TSTART))