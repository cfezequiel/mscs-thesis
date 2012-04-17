function out = cbrunsim(map)
%CBRUNSIM Summary of this function goes here
%   Detailed explanation goes here
     
    % === Start of simulation parameters ===
    wObstacle = 2;
    % === End of simulation parameters ===
    
    % Get search points
    xrange = get(map, 'XLim');
    yrange = get(map, 'YLim');
    pmin = [xrange(1); yrange(1)];
    pmax = [xrange(2); yrange(2)];
    
    % Get map information
    mapInfo = get(map, 'UserData');
    
    % Get starting position
    posStart = getPosition(mapInfo.startpoint)';
    
    % Get obstacle circles
    nObstacleCircles = size(mapInfo.obstacleCircles, 1);
    obstacleCircles = zeros(nObstacleCircles, 4);
    for j = 1:nObstacleCircles
        % Note: obstacleCircles(j, :) = [x y width height]
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
    obstacleLinesAndBoundaries = vertcat(obstacleLines, boundaries);
    
    % Get waypoints (goals)
    nWaypoints = size(mapInfo.waypoints, 1);
    % Include start position as the first 'waypoint'
    waypoints = [posStart zeros(2, nWaypoints)];
    for j = 1:nWaypoints
        waypoints(:, j + 1) = getPosition(mapInfo.waypoints(j))';
    end
    
    % === Start of simulation loop ===
    paths = zeros(1, nWaypoints);
    hold on;
    for i = 1:nWaypoints
        pos = computepath(waypoints(:, i), waypoints(:, i + 1), ...
                          obstacleCircles, obstacleLinesAndBoundaries, ...
                          wObstacle, pmax, pmin);
                               
        % Display each path
        %paths(i) = plot(pos(1, :), pos(2, :), 'r-');
        paths(i) = scatter(pos(1, :), pos(2, :), 'Marker', 'o');
    end
    hold off;
    % === Simulation loop end ===
    
    % Compile simulation info
    X = pmin(1):pmax(1);
    Y = pmin(2):pmax(2);
    simInfo.X = X;
    simInfo.Y = Y;
    simInfo.obstacleCircles = obstacleCircles;
    simInfo.obstacleLines = obstacleLinesAndBoundaries;
    simInfo.wObstacle = wObstacle;
    simInfo.paths = paths;
    
    % Update map data with path/s
    set(map, 'UserData', mapInfo);
    
    % Set output value
    out = simInfo;
end