% Test for the astar() function

% Generate adjacency matrix
G = [0 Inf Inf 4 Inf Inf 2 Inf Inf Inf;
     Inf 0 4 Inf Inf Inf 2 Inf 4 Inf;
     Inf 4 0 4 Inf 2.5 Inf Inf Inf 6;
     4 Inf 4 0 Inf Inf 3 Inf Inf Inf;
     5 2 4.5 5 0 Inf 4.5 3.5 Inf Inf;
     Inf Inf 2.5 Inf 3 0 Inf Inf Inf Inf;
     2 2 Inf 3 Inf Inf 0 2 Inf Inf;
     Inf Inf Inf Inf Inf Inf 2 0 Inf Inf;
     Inf 4 Inf Inf 6 Inf Inf Inf 0 Inf;
     Inf Inf 6 Inf 1 Inf Inf Inf Inf 0];

% Set start and goal nodes
start = 8;
goal = 5;

% Generate heuristic (distance of each node to the goal
h = [5; 2; 4.5; 5; 0; 3; 4.5; 3.5; 6; 1];
 
% Run astar
[dist, path, pred] = astar(G, h, start, goal);

% Verify output
expPath = [8; 7; 2; 3; 6; 5];
if path == expPath
    disp('Test successful');
else
    disp('Test failed');
end
