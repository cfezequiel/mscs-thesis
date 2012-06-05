function runsim(map)
%RUNSIM Run path planning simulation

% ===== Test parameters =====

% Initial number of configurations (NxN)
N = 100;

% ===========================

% ===== Extract map information =====

% Get map information
mapInfo = get(map, 'UserData');

% Get the start position
start = getPosition(mapInfo.startpoint);

% Get goal position/s
goals = getPosition(mapInfo.waypoints);

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
paths = cell(size(goals));
iStart = 1;
for i = 1:size(goals, 1) - 1
    [dist pred] = dijkstra(D, iStart);
    iParent = size(Csg, offset + i);
    
    path = [];
    while (iParent > 0)
        path = [path, iParent];
        iParent = pred(iParent);
    end

    if (pred(size(Csg, 1)) == 0)
        path = [];
    else
        path = fliplr(path);
    end
    
    paths(i) = path;
    
    % Set new starting point
    iStart = iStart + 1;
end

t3 = toc;

% ===== Plot the graph ====

hold on

% Plot the grid of free dots
plot(Cfree(:, 1), Cfree(:, 2), '.', 'MarkerSize', 1);

% Plot shortest path
plot(Csg(path, 1),Csg(path, 2),'b','LineWidth',2)

hold off


% ===== Print performance results =====

fprintf(' - %d collision-free samples were generated in\n %d seconds.\n\n',...
        size(Cfree, 1), t1)
fprintf(' - All node distances were calculated in\n %d seconds.\n\n', t2)
fprintf(' - Dijkstra''s Algorithm finished after\n %d seconds.\n\n', t3)


end

