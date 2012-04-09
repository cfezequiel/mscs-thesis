function cbdeletewaypoint(src, event, map, listbox)
%CBDELETEWAYPOINT Summary of this function goes here
%   Detailed explanation goes here

    mapInfo = get(map, 'UserData');
    if strcmp(mapInfo.state, 'Delete')
        % Remove waypoint from list
        i = mapInfo.waypoints == src;
        mapInfo.waypoints(i) = [];
        
        % Remove the waypoint from listbox
        list = get(listbox, 'String');
        waypointName = get(src, 'DisplayName');
        newList = {};
        j = 1;
        for i = 1:length(list)
            if ~strcmp(list{i}, waypointName)
                newList{j} = list{i};
                j = j + 1;
            end
        end
        set(listbox, 'String', newList);

        % Delete the object
        delete(src);
        set(map, 'UserData', mapInfo);
    end
    
end

