function out = pldist(point, lineSegment)
%PLDIST Summary of this function goes here
%   Detailed explanation goes here

    % Get endpoints of line segment
    a = lineSegment(1:2)';
    b = lineSegment(3:4)';
    
    % Compute square of line segment length
    dLine2 = (a(1) - b(1))^2 + (a(2) - b(2))^2;
    
    % If the line segment is actually just a point,
    % then return the distance between two points
    if dLine2 == 0
        out = pdist([point a]');
        return;
    end
    
    t = ((point - a)' * (b - a))/dLine2;
    if (t < 0) 
        out = pdist([point a]');
        return;
    elseif (t > 1) 
        out = pdist([point b]');
        return;
    end
        
    projection = a + t * (b - a);
    out = pdist([point projection]');
end

