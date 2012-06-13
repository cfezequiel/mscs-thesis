function cbdeletewaypoint(src, event, map, listbox)
%CBDELETEWAYPOINT Summary of this function goes here
%   Detailed explanation goes here

    mapInfo = get(map, 'UserData');
    nWaypoints = size(mapInfo.waypoints, 1);
    wpName = get(src, 'DisplayName');
    if strcmp(mapInfo.state, 'Delete')
        % Remove waypoint from list
        for i = 1:nWaypoints
            if strcmp(wpName, get(mapInfo.waypoints(i), 'DisplayName'))
                mapInfo.waypoints(i) = [];
                break;
            end
        end
        
        % Remove the waypoint from listbox
        list = get(listbox, 'String');
        for i = 1:size(list, 1)
            if strcmp(list{i}, wpName)
                break
            end
        end
        list(i) = [];
        set(listbox, 'String', list);

        % Delete the object
        delete(src);
        set(map, 'UserData', mapInfo);
    end
    disp('kaboodle');
end

