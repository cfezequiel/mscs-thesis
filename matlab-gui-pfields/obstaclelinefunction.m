function out = obstaclelinefunction(point, obstacles, weight)
    
    nObstacles = size(obstacles, 1);
    m = zeros(1, nObstacles);
    C = -0.8;
    for i = 1:nObstacles
        m(i) = exp(C * pldist(point, obstacles(i))^2);
    end
    out = weight * max(m);          
            
end
