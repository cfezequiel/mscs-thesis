function runsim(map)
%RUNSIM Run path planning simulation
%       This uses a modified version of the probabilistic roadmap method
%       (PRM). The configurations are plotted as an NxN grid of points in
%       2D space. Points that are within obstacles are removed. 

% ===== Test parameters =====

% Initial number of configurations (NxN)
N = 100;

% Gap between path and obstacles
gap = 1;

% ===========================

% ===== Extract map information =====

% Get map information
mapInfo = get(map, 'UserData');

% Get the start position
start = getPosition(mapInfo.startpoint);

% Get goal position/s
goals = zeros(size(mapInfo.waypoints, 1), 2);
for i = 1:size(mapInfo.waypoints, 1)
    goals(i, :) = getPosition(mapInfo.waypoints(i));
end

% Get map dimensions
xLim = get(map, 'XLim');
yLim = get(map, 'YLim');

% Get static obstacles
xStatObsExp = mapInfo.staticObstacles.X;
yStatObsExp = mapInfo.staticObstacles.Y;

% Get rectangular obstacles
nObstacleRects = size(mapInfo.obstacleRects, 1);
xObstacleRects = zeros(4, nObstacleRects);
yObstacleRects  = zeros(4, nObstacleRects);
for i = 1:nObstacleRects
    pos = getPosition(mapInfo.obstacleRects(i));
    xmin = pos(1);
    ymin = pos(2);
    w = pos(3) + gap;
    h = pos(4) + gap;
    xObstacleRects(:, i) = [xmin; xmin; xmin + w; xmin + w];
    yObstacleRects(:, i) = [ymin; ymin + h; ymin + h; ymin];
end

% Get circular obstacles
nObstacleCircles = size(mapInfo.obstacleCircles, 1);
angles = deg2rad([0; 45; 90; 135; 180; 225; 270; 315]);
nAngles = size(angles, 1);
xObstacleCircles = zeros(nAngles, nObstacleCircles);
yObstacleCircles = zeros(nAngles, nObstacleCircles);
for i = 1:nObstacleCircles
    pos = getPosition(mapInfo.obstacleCircles(i));
    xmin = pos(1);
    ymin = pos(2);
    w = pos(3);
    % NOTE: No need to extract h (height), h = w for circles
    r = 0.5 * w;
    cx = xmin + r;
    cy = ymin + r;
    xObstacleCircles(:, i) = r .* cos(angles) + cx;
    yObstacleCircles(:, i) = r .* sin(angles) + cy;
end

% ===== Generate the configuration space =====

tic;

% Create grid of dots
xint = xLim(2) / (N + 1);
xv = xint:xint:xLim(2) - xint;
yint = yLim(2) / (N + 1);
yv = yint:yint:yLim(2) - yint;

[X Y] = meshgrid(xv, yv);
X = X(:);
Y = Y(:);

% Create the configuration matrix
C = [X'; Y']';

% Create binary matrix for determining whether point is in/out of polygon
IN = ones(size(C, 1), 1);

% Collision check static obstacles
for i = 1:size(xStatObsExp, 2)
    IN = IN .* ~inpolygon(C(:, 1), C(:, 2), xStatObsExp(:, i), yStatObsExp(:, i));
end

% Collision check rectangular obstacles
for i = 1:size(xObstacleRects, 2)
    IN = IN .* ~inpolygon(C(:, 1), C(:, 2), xObstacleRects(:, i), ...
        yObstacleRects(:, i));
end

% Collision check circular obstacles
for i = 1:size(xObstacleCircles, 2)
    IN = IN .* ~inpolygon(C(:, 1), C(:, 2), xObstacleCircles(:, i), ...
        yObstacleCircles(:, i));
end

% Create the free configuration matrix 
Cfree = [(C(:, 1) .* IN)'; (C(:, 2) .* IN)']';
Cfree(all(Cfree == 0, 2), :) = [];

t1 = toc;

% ===== Build the roadmap =====

tic;

% Combine the coordinate matrix with start and end nodes
Csg = [start; Cfree; goals]; 

% Generate the distance matrix
D = pdist2(Csg, Csg);

% Set the nearest neighbor radius
r = sqrt(xint^2 + yint^2);

% Generate the adjacency matrix
A = D;
A(A > r) = Inf;

t2 = toc;

% ===== Get shortest paths =====

tic;

% Generate shortest paths
offset = 1 + size(Cfree, 1);
paths = cell(size(goals, 1), 1);
iStart = 1;
for i = 1:size(goals, 1)
    
    % ==== Use A* algorithm to get shortest path  ====
    % Generate heuristic matrix
    iGoal = offset + i;
    h = D(iGoal, :);
    [~, path, ~] = astar(A, h, iStart, iGoal);
    
    paths{i} = path;
    
    % Set new starting point
    iStart = iGoal;
end

t3 = toc;

% ===== Plot the simulation data on the map ====

hold on

% Plot the grid of free dots
mapInfo.cFree = plot(map, Cfree(:, 1), Cfree(:, 2), '.', 'MarkerSize', 1);

% Plot paths
for i = 1:size(paths, 1)
    h = plot(map, Csg(paths{i}, 1),Csg(paths{i}, 2),'b','LineWidth',2);
    mapInfo.paths = [mapInfo.paths; h];
end

hold off


% ===== Print performance results =====

fprintf(' - %d collision-free samples were generated in\n %d seconds.\n\n',...
        size(Cfree, 1), t1)
fprintf(' - All node distances were calculated in\n %d seconds.\n\n', t2)
fprintf(' - A-star Algorithm finished after\n %d seconds.\n\n', t3)


% ===== Update global map information =====

set(map, 'UserData', mapInfo);

end

