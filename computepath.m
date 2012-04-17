function out = computepath(start, goal, obstacleCircles, obstacleLines, ...
                           wObstacle, pmax, pmin)
%COMPUTEPATH Summary of this function goes here
%   Detailed explanation goes here

    % === start: Simulation Parameters ===
    % Set weights
    wGoal = 1.0000e-04;
    
    % Number of points on a circular pattern to sense
    nSensePoints = 16;
    
    % Sensing radius
    rSense = 1;
    
    % Maximum number of steps to produce
    nSteps = 500;      
    
    % Step size to take in chosen direction at each move
    lambda = 1;
    % === end: Simulation Parameters ===

    % Position matrix
    path = start;
    path(:, 2:nSteps) = zeros(2, nSteps - 1);
    
    % Sense points
    sensePoints = zeros(2, nSensePoints);
    
    % Initialize memory
    Jo = zeros(nSensePoints, 1);
    Jg = zeros(nSensePoints, 1);
    J = zeros(nSensePoints, 1);
    
    % Initialize angles to be used around the circle
    theta = zeros(nSensePoints, 1);
    for i = 2:nSensePoints
        theta(i, 1) = theta(i - 1, 1) + ((2 * pi) / nSensePoints);
    end
    
    % Iterate over number of steps
    for j = 1:nSteps

        % Use projection to keep in boundaries (like hitting a wall and staying at it)
        path(:, j) = min(path(:, j), pmax);
        path(:, j) = max(path(:, j), pmin);

        % Sense points on circular pattern
        for k = 1:nSensePoints
            % Point on circular pattern
            sensePoints(:, k) = [path(1, j) + rSense * cos(theta(k, 1)); ...
                                 path(2, j) + rSense * sin(theta(k, 1))];
            % Compute the obstacle function 
            % (What is sensed at each sensed point)
            Jo(k, 1) = obstaclefunction(sensePoints(:, k), ... 
                                        obstacleCircles, ...
                                        obstacleLines, ...
                                        wObstacle);
            % Compute how well each point moves toward the goal
            Jg(k, 1) = goalfunction(sensePoints(:, k), ...
                                    goal, ...
                                    wGoal);
            
            % Compute function for optimization in planning
            J(k,1) = Jo(k,1) + Jg(k,1); 
        end

        % Next pick the best direction
        [val, bestOne] = min(J);

        % Then, update the vehicle position 
        % (Pick best direction and move step of lambda that way)
        path(:, j + 1) = [path(1, j) + lambda * cos(theta(bestOne, 1)); ...
                         path(2, j) + lambda * sin(theta(bestOne, 1))];

    end
    
    out = path;
   
end

