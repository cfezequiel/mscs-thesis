function runsim(map)
%RUNSIM Run path planning simulation
%       This uses a modified version of the probabilistic roadmap method
%       (PRM). The configurations are plotted as an NxN grid of points in
%       2D space. Points that are within obstacles are removed. 

% ===== Test parameters =====

% Initial number of configurations (NxN)
N = 150;

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

% Create the free configuration matrix
IN = ones(size(C, 1), 1);
for i = 1:size(xStatObsExp, 2)
    IN = IN .* ~inpolygon(C(:, 1), C(:, 2), xStatObsExp(:, i), yStatObsExp(:, i));
end
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

% Set distances of 'non-near' neighbors to infinite
D(D > r) = Inf;

t2 = toc;


% ===== Get shortest paths =====

tic;

% Generate shortest paths
offset = 1 + size(Cfree, 1);
paths = cell(size(goals), 1);
iStart = 1;
for i = 1:size(goals, 1)
    
    % Use Dijkstra's algorithm to get all shortest paths from start pos
    [~, pred] = dijkstra(D, iStart);
    
    
    % Build the path vector from end to start
    iEnd = offset + i;
    iParent = iEnd;
    onePath = [];
    while (iParent > 0)
        onePath = [onePath, iParent];
        iParent = pred(iParent);
    end
    
    % Flip the path vector so it goes from start to end
    onePath = fliplr(onePath);
    
    % Add path to list of paths
    paths{i} = onePath;
    
    % Set new starting point
    iStart = iEnd;
end

t3 = toc;

% ===== Plot the graph ====

hold on

% Plot the grid of free dots
plot(Cfree(:, 1), Cfree(:, 2), '.', 'MarkerSize', 1);

% Plot  paths
for i = 1:size(paths, 1)
    plot(Csg(paths{i}, 1),Csg(paths{i}, 2),'b','LineWidth',2)
end

hold off


% ===== Print performance results =====

fprintf(' - %d collision-free samples were generated in\n %d seconds.\n\n',...
        size(Cfree, 1), t1)
fprintf(' - All node distances were calculated in\n %d seconds.\n\n', t2)
fprintf(' - Dijkstra''s Algorithm finished after\n %d seconds.\n\n', t3)


end

