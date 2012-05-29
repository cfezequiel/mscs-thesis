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
          
    % Iterate over circular obstacles
    if ~isempty(obstacleCircles)
        R = obstacleCircles(:, 3) ./ 2;
        X = obstacleCircles(:, 1) + R;
        Y = obstacleCircles(:, 2) + R;
        C = 9.75 / (2 .* R.^2);
        Xp = point(1);
        Yp = point(2);
        JC = exp(-C .* ((Xp - X).^2 .* (Yp - Y).^2));
        disp(JC);
    else
        JC = 0;
    end
    
    %FIXME: lines have uniform thickness?
    C = 10;
    JL = zeros(nObstacleLines, 1);
    for j = 1:nObstacleLines
        JL(j) = exp(-C * pldist(point, obstacleLines(j, :))^2);
    end
    out = weight * max([JC; JL]);
            
end
