% ===== Test parameters =====

% Determine total number of dots as NxN
% NOTE: at N=100, out-of-memory error was encountered
N = 50;

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

% Detect collisions
IN = ones(size(X, 1), 1);
for i = 1:size(xStatObs, 2)
    IN = IN .* ~inpolygon(X, Y, xStatObs(:, i), yStatObs(:, i));
end
X = X .* IN;
Y = Y .* IN;

% Create the coordinate matrix
C = [X'; Y']';
C = sparse(C);

t1 = toc;


% ===== Build the roadmap =====

% Combine the coordinate matrix with start and end nodes
Csg = [start(1), start(2); C; goal(1), goal(2)]; 

tic;

% Generate the distance matrix
D = pdist2(Csg, Csg);

t2 = toc;

% Get the nearest neighbor radius
r = sqrt(xint^2 + yint^2);

% Set distances of 'non-near' neighbors to infinite
D(D > r) = Inf;

tic;


% ===== Get shortest path =====

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
scatter(X, Y, 5/N);

% Plot shortest path
plot(Csg(path,1),Csg(path,2),'b','LineWidth',2)

hold off

% ===== Print performance results =====

fprintf(' - %d collision-free samples were generated in\n %d seconds.\n\n',...
        size(C, 1), t1)
fprintf(' - All node distances were calculated in\n %d seconds.\n\n', t2)
fprintf(' - Dijkstra''s Algorithm finished after\n %d seconds.\n\n', t3)


