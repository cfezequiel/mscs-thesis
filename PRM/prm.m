% ===== Parameters =====
resolution = 50;
k = 10;
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
        
% Plot static workspace
figure(1);
axis([0 xmax 0 ymax]);
hold on
patch(xStatObs, yStatObs, 'g');


% ===== Generate the configuration space =====

% Create grid of dots
xint = xmax / (resolution + 1);
xv = xint:xint:xmax - xint;
yint = ymax / (resolution + 1);
yv = yint:yint:ymax - yint;

[X Y] = meshgrid(xv, yv);
X = X(:);
Y = Y(:);


% ===== Build the roadmap =====

% Detect collisions
IN = ones(size(C, 1), 1);
for i = 1:size(xStatObs, 2)
    IN = IN .* ~inpolygon(X, Y, xStatObs(:, i), yStatObs(:, i));
end
X = X .* IN;
X = sparse(X);
Y = Y .* IN;
Y = sparse(Y);

% Plot the grid of free dots
scatter(X, Y, 5/resolution);

% Generate distance matrix;
C = [X'; Y']';
D = pdist2(C, C);

% Plot the line segments corresponding to non-colliding pairs of dots
%TODO

% ===== Get shortest path =====

% Use Dijkstra's algorithm to get shortest path
[dist pred] = dijkstra(D, 1);




