function cbrunsim(map)
%CBRUNSIM Summary of this function goes here
%   Detailed explanation goes here

    % === start: Simulation Parameters ===
    % Set weights
    wObstacleCircle = 1;
    wObstacleLine = 1;
    wObstacle = 1;
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
    
    % Get search points
    xrange = get(map, 'XLim');
    yrange = get(map, 'YLim');
    pmin = [xrange(1); yrange(1)];
    pmax = [xrange(2); yrange(2)];
    X = pmin(1):pmax(1);
    Y = pmin(2):pmax(2);
    
    % Get map information
    mapInfo = get(map, 'UserData');
    
    % Get starting position
    posStart = getPosition(mapInfo.startpoint)';
    
    % Get obstacle circles
    nObstacleCircles = size(mapInfo.obstacleCircles, 1);
    obstacleCircles = zeros(nObstacleCircles, 4);
    for j = 1:nObstacleCircles
        % Note: obstacleCircle(i) = [x y width height]
        obstacleCircles(j, :) = getPosition(mapInfo.obstacleCircles(j));
    end
    
    % Get obstacle lines
    nObstacleLines = size(mapInfo.obstacleLines, 1);
    obstacleLines = zeros(nObstacleLines, 4);
    for j = 1:nObstacleLines
        posTmp = getPosition(mapInfo.obstacleLines(j));
        % Note: pos = [x1 y1; x2 y2]
        % But we need to store it as [x1 y1 x2 y2]
        obstacleLines(j, :) = reshape(posTmp', 1, 4);
    end
    
    % Get boundary lines
    nBoundaries = size(mapInfo.boundaries, 1);
    boundaries = zeros(nBoundaries, 4);
    for j = 1:nBoundaries;
       X = get(mapInfo.boundaries(j), 'XData');
       Y = get(mapInfo.boundaries(j), 'YData');
       boundaries(j, :) = [X(1) Y(1) X(2) Y(2)];
    end
    
    % Combine obstacle lines and boundaries 
    linesAndBoundaries = vertcat(obstacleLines, boundaries);
    
    % Get waypoints (goals)
    nWaypoints = size(mapInfo.waypoints, 1);
    waypoints = zeros(nWaypoints, 2);
    for j = 1:nWaypoints
        waypoints(j, :) = getPosition(mapInfo.waypoints(j));
    end
       
    % --- Start of simulation loop ---
    % Position matrix
    pos = posStart;
    pos(:, 2:nSteps) = zeros(2, nSteps - 1);
    
    % Sense points
    sensePoints = zeros(2, nSensePoints);
    
    % Initialize memory
    Joc = zeros(nSensePoints, 1);
    Jg = zeros(nSensePoints, 1);
    J = zeros(nSensePoints, 1);
    
    % Initialize angles to be used around the circle
    theta(:, 1) = zeros(nSensePoints, 1);
    for i = 2:nSensePoints
        theta(i, 1) = theta(i - 1, 1) + ((2 * pi) / nSensePoints);
    end
    
    % FIXME: temporarily set only one goal (the first waypoint)
    pGoal = getPosition(mapInfo.waypoints(1));
    
    % Iterate over number of steps
    for j = 1:nSteps

        % Use projection to keep in boundaries (like hitting a wall and staying at it)
        pos(:, j) = min(pos(:, j), pmax);
        pos(:, j) = max(pos(:, j), pmin);

        % Sense points on circular pattern
        for k = 1:nSensePoints
            % Point on circular pattern
            sensePoints(:, k) = [pos(1, j) + rSense * cos(theta(k, 1)); ...
                                 pos(2, j) + rSense * sin(theta(k, 1))];
            % Compute the obstacle function 
            % (What is sensed at each sensed point)
            Joc(k, 1) = obstaclefunction(sensePoints(:, k), ... 
                                         obstacleCircles, ...
                                         linesAndBoundaries, ...
                                         wObstacle);
            % Compute how well each point moves toward the goal
            Jg(k, 1) = goalfunction(sensePoints(:, k), ...
                                    pGoal, ...
                                    wGoal);
            
            % Compute function for optimization in planning
            J(k,1) = Joc(k,1) + Jg(k,1); 
        end

        % Next pick the best direction
        [val, bestOne] = min(J);

        % Then, update the vehicle position 
        % (Pick best direction and move step of lambda that way)
        pos(:, j + 1) = [pos(1, j) + lambda * cos(theta(bestOne, 1)); ...
                         pos(2, j) + lambda * sin(theta(bestOne, 1))];

    end
    % --- Simulation loop end ---
    
    % Plot the simulated path/s
    hold on
    mapInfo.paths(1) = plot(pos(1, :), pos(2, :), 'r-');
    hold off
    
    % Update map data with path/s
    set(map, 'UserData', mapInfo);
    
end