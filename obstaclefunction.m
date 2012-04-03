function out = obstaclefunction(X, Y, obstacles, weight)
    
    nX = size(X, 2);
    nY = size(Y, 2);
    nObstacles = size(obstacles, 1);
    m = zeros(1, nObstacles);
    
    % Constant factor
    % What is this for?
    c = -0.8;
    
    v = zeros(nY, nX);
    for i = 1:nX
        for j = 1:nY
            for k = 1:nObstacles
                xo = obstacles(k, 1);
                yo = obstacles(k, 2);
                m(k) = exp(c * ((X(i) - xo)^2 + (Y(j) - yo)^2));
            end
            v(j, i) = weight * max(m);
        end
    end
    out = v;
    

