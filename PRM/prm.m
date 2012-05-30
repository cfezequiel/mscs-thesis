% ===== Parameters =====
nSamples = 1000;
k = 10;
start = [3 4];
goal = [140 210];


% ===== Construct the static workspace =====

% Define the static workspace boundaries
xmax = 149;
ymax = 215;

% Define the vertices of the static obstacles
xpatch = [7 7 61;
          7 7 61;
          54 54 149;
          54 54 149];
ypatch = [7 111 0;
          104 208 208;
          104 208 208;
          7 111 0];

% Generate random dots (x,y) on the workspace
randX = randi([1 xmax - 1], [nSamples, 1]);
randY = randi([1 ymax - 1], [nSamples, 1]);

% Filter out dots that lie on the obstacles
IN = ones(size(randX));
for i = 1:size(xpatch, 2)
    IN = IN .* ~inpolygon(randX, randY, xpatch(:, i), ypatch(:, i));
end
X = randX(randX .* IN ~= 0);
Y = randY(randY .* IN ~= 0);

% Plot the workspace
h = figure(1);
hold on
patch(xpatch, ypatch, 'r');
axis([0 xmax 0 ymax]);
scatter(X, Y);


% ===== Build the roadmap =====

% Create point matrix
M = [X'; Y']';
nP = size(M, 1);

% Create point-pairwise distance matrix
D = pdist2(M, M);

% Get indices of k nearest neighbors for each point
[sorted, indices] = sort(D, 2);
I = indices(:, 2:k + 1);

% Create adjacency matrix
A = zeros(nP, nP);

% Generate edges
collision = 0;
for i = 1:size(I, 1)
    q = M(i, :);
    for j = 1:size(I, 2)
        p = M(I(j), :);
        % Do collision detection
        for k = 1:size(xpatch, 2)
            [xi yi] = polyxpoly([q(1) p(1)], [q(2) p(2)], xpatch(:, k)', ypatch(:, k)');
            if ~isempty(xi) || ~isempty(yi)
                collision = 1;
                break
            end
        end
        if collision == 0
            line([q(1) p(1)], [q(2) p(2)]);
            A(i, I(j)) = 1;
        end
        collision = 0;
    end
end


% ===== Get shortest path =====

% Find nearest neighbors to start and goal
d = pdist2([start; goal], M);
[sorted, nearStartGoal] = sort(d, 2);

nearStart = nearStartGoal(1, 1);
nearGoal = nearStartGoal(2, 1);

% Use Dikjstra's Algorithm to calculate shortest path to goal
[dist, pred] = dijkstra(sparse(D), nearStart);

parent = nearGoal;

path = [];
while (parent > 0)
    path = [path, parent];
    parent = pred(parent);
end

if (pred(nearGoal) == 0)
    path = [];
else
    path = fliplr(path);
end


hold off

