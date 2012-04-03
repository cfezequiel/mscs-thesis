function cbrunsim(map)
%CBRUNSIM Summary of this function goes here
%   Detailed explanation goes here

    % === start: Simulation Parameters ===
    % Set weights
    wObstacle = 1;
    wGoal = 1.0000e-04;
    
    % Maximum number of steps to produce
    nSteps = 500;            
    % === end: Simulation Parameters ===
    
    % Get starting position
    % TODO: get value from map
    startPos = [10; 10];
    
    % Get search points
    xrange = get(map, 'XLim');
    yrange = get(map, 'YLim');
    X = xrange(1):xrange(2);
    Y = yrange(1):yrange(2);
    
    % Get map data
    mapData = get(map, 'UserData');
    
    % Get obstacle points
    nObstacles = size(mapData.obstacles, 2);
    obstacles = zeros(nObstacles, 2);
    for i = 1:nObstacles
        x = get(mapData.obstacles(i), 'XData');
        y = get(mapData.obstacles(i), 'YData');
        obstacles(i, :) = [x(1) y(1)];
    end
        
    % Get waypoints (goals)
    nWaypoints = size(mapData.waypoints, 2);
    waypoints = zeros(nWaypoints, 2);
    for i = 1:nWaypoints
        x = get(mapData.waypoints(i), 'XData');
        y = get(mapData.waypoints(i), 'YData');
        waypoints(i, :) = [x(1) y(1)];
    end
    
    % Get obstacle function values
    Z1 = obstaclefunction(X, Y, obstacles, wObstacle);
    
    % Get goal function values
    Z2 = goalfunction(X, Y, waypoints(1, :), wGoal);

    % Plot path
    contour(X, Y, Z1+Z2,50);
    colormap(jet);
    
end