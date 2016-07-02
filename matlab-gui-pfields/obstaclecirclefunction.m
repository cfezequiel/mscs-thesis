function out = obstaclecirclefunction(x, y, obstacles, weight)
%OBSTACLECIRCLEFUNCTION Summary of this function goes here
%   Detailed explanation goes here

    nObstacles = size(obstacles, 1);
    
    % Return 0 if there are no obstacles
    if ~nObstacles
        out = 0;
        return;
    end
    
    m = zeros(1, nObstacles);
    
    % Constant factor
    % What is this for?
    C = -0.8;

    for k = 1:nObstacles
        xo = obstacles(k, 1);
        yo = obstacles(k, 2);
        m(k) = exp(C * ((x - xo)^2 + (y - yo)^2));
    end
    
    out = weight * max(m);
    
end

