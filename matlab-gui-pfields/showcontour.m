function out = showcontour(X, Y, obstacleCircles, obstacleLines, weight)
%SHOWCONTOUR Summary of this function goes here
%   Detailed explanation goes here
    
    nY = length(Y);
    nX = length(X);
    
    Z = zeros(nY, nX);
    for i = 1:length(X)
        for j = 1:length(Y)
            Z(j, i) = obstaclefunction([X(i); Y(j)], obstacleCircles, ...
                                       obstacleLines, weight);
        end
    end
    
    [C, out] = contour(X, Y, Z, 25);
end
