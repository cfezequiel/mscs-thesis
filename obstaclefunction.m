function out = obstaclefunction(point, obstacleCircles, obstacleLines, weight)

    % Get obstacle counts
    nObstacleCircles = size(obstacleCircles, 1);
    nObstacleLines = size(obstacleLines, 1);
    nObstacles = nObstacleCircles + nObstacleLines;
    
    % Return 0 if there are no obstacles
    if ~nObstacles 
        out = 0;
        return;
    end
    
    % Allocate memory
    m = zeros(1, nObstacles);
       
    % Iterate over circular obstacles
    for i = 1:nObstacleCircles
        % Radius (using elliptic width)
        r = obstacleCircles(i, 3) / 2;
        % Get the center
        % Note: obstacle position (x,y) represents the lower-left point
        % of the imaginary 'bounding box' that surrounds the circle
        xo = obstacleCircles(i, 1) + r;
        yo = obstacleCircles(i, 2) + r;
        C = (9.75 / (2 * r^2));
        m(i) = exp(-C * ((point(1) - xo)^2 + (point(2) - yo)^2));
    end
    
    % Iterate over linear obstacles
    i = nObstacleCircles;
    %FIXME: lines have uniform thickness?
    C = 10;
    for j = 1:nObstacleLines
        m(i + j) = exp(-C * pldist(point, obstacleLines(j, :))^2);
    end
    out = weight * max(m);
            
end
