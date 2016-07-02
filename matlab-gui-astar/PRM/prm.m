% ===== Test parameters =====

% Initial number of configurations (NxN)
N = 50;

% Determine total number of free configurations
nFree = 2000;

% Gap between robot and obstacles
gap = 1.5;

% Start and goal points
start = [3 4];
goal = [140 210];


% ===== Construct the static workspace =====

% Define the static workspace boundaries
xmax = 149;
ymax = 215;

% Define the vertices of the static obstacles
xStatObs = [7 7 61;
            7 7 61;
            54 54 149;
            54 54 149];
yStatObs = [7 111 0;
            104 208 208;
            104 208 208;
            7 111 0];
        
% Expand the obstacles to avoid the robot bumping into them
xStatObsExp = xStatObs;
xStatObsExp([1,2,5,6,9,10]) = xStatObsExp([1,2,5,6,9,10]) - gap;
xStatObsExp([3,4,7,8,11,12]) = xStatObsExp([3,4,7,8,11,12]) + gap;
yStatObsExp = yStatObs;
yStatObsExp([1,4,5,8,9,12]) = yStatObsExp([1,4,5,8,9,12]) - gap;
yStatObsExp([2,3,6,7,10,11]) = yStatObsExp([2,3,6,7,10,11]) + gap;

% ===== Generate the configuration space =====

tic;

% Create grid of dots
xint = xmax / (N + 1);
xv = xint:xint:xmax - xint;
yint = ymax / (N + 1);
yv = yint:yint:ymax - yint;

[X Y] = meshgrid(xv, yv);
X = X(:);
Y = Y(:);

% Create the configuration matrix
C = [X'; Y']';

% Create the free configuration matrix
Cfree = zeros(nFree, 2);
iC = 0;
collision = false;
while (iC < nFree)
    i = randi(size(C, 1), 1);
    c = C(i, :);
    for j = 1:size(xStatObsExp, 2)
        if inpolygon(c(1), c(2), xStatObsExp(:, j), yStatObsExp(:, j))
            collision = true;
        end
    end
    if ~collision
        iC = iC + 1;
        Cfree(iC, :) = c;
    end
    collision = false;
end

t1 = toc;


% ===== Build the roadmap =====

tic;

% Combine the coordinate matrix with start and end nodes
Csg = [start(1), start(2); Cfree; goal(1), goal(2)]; 

% Generate the distance matrix
D = pdist2(Csg, Csg);

% Set the nearest neighbor radius
r = sqrt(xint^2 + yint^2);

% Set distances of 'non-near' neighbors to infinite
D(D > r) = Inf;

t2 = toc;


% ===== Get shortest path =====

tic;

% Use Dijkstra's algorithm to get shortest path
[dist pred] = dijkstra(D, 1);

t3 = toc;

% Generate final path
parent = size(Csg,1);
path = [];

while (parent > 0)
    path = [path, parent];
    parent = pred(parent);
end

if (pred(size(Csg,1)) == 0)
    path = [];
else
    path = fliplr(path);
end


% ===== Plot the graph ====

hold on

% Plot static workspace
figure(1);

axis([0 xmax 0 ymax]);
patch(xStatObs, yStatObs, 'g');

% Plot start and end nodes
plot(start(1), start(2), 'or', 'MarkerSize',10)
plot(goal(1), goal(2), 'xr', 'MarkerSize',10)

% Plot the grid of free dots
plot(Cfree(:, 1), Cfree(:, 2), '.', 'MarkerSize', 1);

% Plot shortest path
plot(Csg(path,1),Csg(path,2),'b','LineWidth',2)

hold off


% ===== Print performance results =====

fprintf(' - %d collision-free samples were generated in\n %d seconds.\n\n',...
        size(Cfree, 1), t1)
fprintf(' - All node distances were calculated in\n %d seconds.\n\n', t2)
fprintf(' - Dijkstra''s Algorithm finished after\n %d seconds.\n\n', t3)


