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
    
    % Constant factor
    % What is this for?
    C = -0.8;
   
    % Iterate over circular obstacles
    for i = 1:nObstacleCircles
        xo = obstacleCircles(i, 1);
        yo = obstacleCircles(i, 2);
        m(i) = exp(C * ((point(1) - xo)^2 + (point(2) - yo)^2));
    end
    
    % Iterate over linear obstacles
    i = nObstacleCircles;
    for j = 1:nObstacleLines
        m(i + j) = exp(C * pldist(point, obstacleLines(j, :))^2);
    end
    out = weight * max(m);
            
end
