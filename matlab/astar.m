function [dist, path, pred] = astar(G, h, start, goal)
%ASTAR Computes shortest distance using the A* (A-star) algorithm
%   Input:
%       G       Graph represented as an adjacency cost matrix (NxN)
%       h       Heuristic cost estimate
%               (In this function, required input is a vector containing
%               distances from each node in G to the goal)
%       start   Index of start node in G
%       goal    Index of goal node in G
%   Output:
%       dist    Shortest distance value
%       path    Vector containing the path from start to goal
%       pred    Vector containing parent of each node

    % ===== Initialize =====
    nG = size(G, 1);
    
    % Set timeout in case of bugs
    timeout = nG * nG;
    
    % Distance between neighbor nodes
    d = G;
    
    closed = [];
    open = start;
    path = [];
    pred = zeros(nG, 1);
    g = zeros(nG, 1);
    f = zeros(nG, 1);
    f(start) = g(start) + h(start);
    
    while ~isempty(open)
        % Get the node in the open set having the lowest f value
        current = minf(f, open);
        
        % If current is the goal, reconstruct the path
        % FIXME: this method may have some bugs
        if current == goal
            path = reconstructpath(pred, goal);
            break;
        end

        % Remove current from openset
        open(find(open == current)) = [];
        
        % Add current to closed set
        closed = [closed; current];
        
        % Get neighbors of current node
        neighbors = getneighbors(G, current);
        for j = 1:length(neighbors)
            neighbor = neighbors(j);
            % if neighbor in closed set, next neighbor
            if ~isempty(find(closed == neighbor))
                continue
            end
            
            tmpg = g(current) + d(current, neighbor);
            
            if isempty(find(open == neighbor)) || tmpg < g(neighbor)
                open = [open; neighbor];
                pred(neighbor) = current;
                g(neighbor) = tmpg;
                f(neighbor) = g(neighbor) + h(neighbor);
            end
        end
        
        timeout = timeout - 1;
        if timeout == 0
            disp('Algorithm timed out');
            break;
        end
    end
    
    % Calculate distance value of shortest path
    dist = 0;
    for j = 1:size(path, 1) - 1
        u = path(j);
        v = path(j +1);
        dist = dist + d(u, v);
    end
end

function V = getneighbors(G, u)
% Get the neighbors of each node

    A = G(u, :);
    [B, V] = sort(A);
    B(B == Inf) = 0;
    B(B > 0) = 1;
    V = V .* B;
    V(V == 0) = [];
    
end

function c = minf(f, open)

    a = ones(size(f)) .* Inf;
    a(open) = 1;
    b = a .* f;
    [~, ix] = sort(b);
    c = ix(1);
end
 
function path = reconstructpath(pred, goal)
% Generate the path from a goal node to the start

    reversePath = [goal];
    parent = pred(goal);
    while parent
        reversePath = [reversePath; parent];
        parent = pred(parent);
    end
    
    path = flipud(reversePath);
end





    

