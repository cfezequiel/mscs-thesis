function cbrunsim(map)
%CBRUNSIM Summary of this function goes here
%   Detailed explanation goes here

    % Get search points
    xrange = get(map, 'XLim');
    yrange = get(map, 'YLim');
    
    pmin = [xrange(1); yrange(1)]
    pmax = [xrange(2); yrange(2)]
    

    mapData = get(map, 'UserData');
    
    % Get obstacle points
    
    % Get waypoints (goals)

    % Compute obstacle function
    
    % Compute goal function
    
    % Plot path
    
end

