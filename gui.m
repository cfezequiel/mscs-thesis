function varargout = gui(varargin)
% GUI MATLAB code for gui.fig
%      GUI, by itself, creates a new GUI or raises the existing
%      singleton*.
%
%      H = GUI returns the handle to a new GUI or the handle to
%      the existing singleton*.
%
%      GUI('CALLBACK',hObject,eventData,handles,...) calls the local
%      function named CALLBACK in GUI.M with the given input arguments.
%
%      GUI('Property','Value',...) creates a new GUI or raises the
%      existing singleton*.  Starting from the left, property value pairs are
%      applied to the GUI before gui_OpeningFcn gets called.  An
%      unrecognized property name or invalid value makes property application
%      stop.  All inputs are passed to gui_OpeningFcn via varargin.
%
%      *See GUI Options on GUIDE's Tools menu.  Choose "GUI allows only one
%      instance to run (singleton)".
%
% See also: GUIDE, GUIDATA, GUIHANDLES

% Edit the above text to modify the response to help gui

% Last Modified by GUIDE v2.5 20-Jun-2012 15:28:07

% Begin initialization code - DO NOT EDIT
gui_Singleton = 1;
gui_State = struct('gui_Name',       mfilename, ...
                   'gui_Singleton',  gui_Singleton, ...
                   'gui_OpeningFcn', @gui_OpeningFcn, ...
                   'gui_OutputFcn',  @gui_OutputFcn, ...
                   'gui_LayoutFcn',  [] , ...
                   'gui_Callback',   []);
if nargin && ischar(varargin{1})
    gui_State.gui_Callback = str2func(varargin{1});
end

if nargout
    [varargout{1:nargout}] = gui_mainfcn(gui_State, varargin{:});
else
    gui_mainfcn(gui_State, varargin{:});
end
% End initialization code - DO NOT EDIT


% --- Executes just before gui is made visible.
function gui_OpeningFcn(hObject, eventdata, handles, varargin)
% This function has no output args, see OutputFcn.
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
% varargin   command line arguments to gui (see VARARGIN)

% Choose default command line output for gui
handles.output = hObject;

% Update handles structure
guidata(hObject, handles);

% UIWAIT makes gui wait for user response (see UIRESUME)
% uiwait(handles.figure1);

% ===== Map information =====
mapInfo.state = 'Select';
mapInfo.startpoint = [];
mapInfo.waypoints = [];
mapInfo.nextWaypointIndex = 0;
mapInfo.staticObstacles = struct('X', [], 'Y', []);
mapInfo.obstacleCircles = [];
mapInfo.obstacleRects = [];
mapInfo.paths = [];
mapInfo.cFree = [];
set(handles.axesMap, 'ButtonDownFcn', []);

% Add fixed boundaries
mapInfo = setstaticboundaries(mapInfo);

% Store the map information in the axes
set(handles.axesMap, 'UserData', mapInfo);

% ===========================


% --- Outputs from this function are returned to the command line.
function varargout = gui_OutputFcn(hObject, eventdata, handles) 
% varargout  cell array for returning output args (see VARARGOUT);
% hObject    handle to figure
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Get default command line output from handles structure
varargout{1} = handles.output;


% --------------------------------------------------------------------
function tbppSelect_ClickedCallback(hObject, eventdata, handles)
% hObject    handle to tbppSelect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Disable other path planning toggle tools
    set(handles.tbppSetStartPoint, 'State', 'off');
    set(handles.tbppAddWaypoint, 'State', 'off');
    set(handles.tbppAddObstacleCircle, 'State', 'off');
    set(handles.tbppAddObstacleRect, 'State', 'off');
    set(handles.tbppDelete, 'State', 'off');

% --------------------------------------------------------------------
function tbppSetStartPoint_ClickedCallback(hObject, eventdata, handles)
% hObject    handle to tbppSetStartPoint (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Disable other path planning toggle tools
    set(handles.tbppSelect, 'State', 'off');
    set(handles.tbppAddWaypoint, 'State', 'off');
    set(handles.tbppAddObstacleCircle, 'State', 'off');
    set(handles.tbppAddObstacleRect, 'State', 'off');
    set(handles.tbppDelete, 'State', 'off');
    
% --------------------------------------------------------------------
function tbppAddWaypoint_ClickedCallback(hObject, eventdata, handles)
% hObject    handle to tbppAddWaypoint (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)
    
    % Disable other path planning toggle tools
    set(handles.tbppSetStartPoint, 'State', 'off');
    set(handles.tbppSelect, 'State', 'off');
    set(handles.tbppAddObstacleCircle, 'State', 'off');
    set(handles.tbppAddObstacleRect, 'State', 'off');
    set(handles.tbppDelete, 'State', 'off');

% --------------------------------------------------------------------
function tbppAddObstacleCircle_ClickedCallback(hObject, eventdata, handles)
% hObject    handle to tbppAddObstacleCircle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Disable other path planning toggle tools
    set(handles.tbppSetStartPoint, 'State', 'off');
    set(handles.tbppSelect, 'State', 'off');
    set(handles.tbppAddWaypoint, 'State', 'off');
    set(handles.tbppAddObstacleRect, 'State', 'off');
    set(handles.tbppDelete, 'State', 'off');

% --------------------------------------------------------------------
function tbppDelete_ClickedCallback(hObject, eventdata, handles)
% hObject    handle to tbppDelete (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Disable other path planning toggle tools
    set(handles.tbppSetStartPoint, 'State', 'off');
    set(handles.tbppSelect, 'State', 'off');
    set(handles.tbppAddWaypoint, 'State', 'off');
    set(handles.tbppAddObstacleCircle, 'State', 'off');
    set(handles.tbppAddObstacleRect, 'State', 'off');
    
% --- Executes on selection change in lbWaypoints.
function lbWaypoints_Callback(hObject, eventdata, handles)
% hObject    handle to lbWaypoints (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: contents = cellstr(get(hObject,'String')) returns lbWaypoints contents as cell array
%        contents{get(hObject,'Value')} returns selected item from lbWaypoints


% --- Executes during object creation, after setting all properties.
function lbWaypoints_CreateFcn(hObject, eventdata, handles)
% hObject    handle to lbWaypoints (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: listbox controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end


% --------------------------------------------------------------------
function tbppSelect_OnCallback(hObject, eventdata, handles)
% hObject    handle to tbppSelect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Update the state
    mapInfo = get(handles.axesMap, 'UserData');
    mapInfo.state = 'Select';
    set(handles.axesMap, 'UserData', mapInfo);
    
    % Set 'button down' function of axesMap
    set(handles.axesMap, 'ButtonDownFcn', []);
    
% --------------------------------------------------------------------
function tbppSetStartPoint_OnCallback(hObject, eventdata, handles)
% hObject    handle to tbppSetStartPoint (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Update the state
    mapInfo = get(handles.axesMap, 'UserData');
    mapInfo.state = 'StartPoint';
    set(handles.axesMap, 'UserData', mapInfo);
    
    % Set 'button down' function of axesMap
    set(handles.axesMap, 'ButtonDownFcn', {@cbaddstartpoint, handles});
    
% --------------------------------------------------------------------
function tbppAddWaypoint_OnCallback(hObject, eventdata, handles)
% hObject    handle to tbppAddWaypoint (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Update the state
    mapInfo = get(handles.axesMap, 'UserData');
    mapInfo.state = 'Waypoint';
    set(handles.axesMap, 'UserData', mapInfo);
    
    % Set 'button down' function of axesMap
    set(handles.axesMap, 'ButtonDownFcn', {@cbaddwaypoint, handles});
    
% --------------------------------------------------------------------
function tbppAddObstacleCircle_OnCallback(hObject, eventdata, handles)
% hObject    handle to tbppAddObstacleCircle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Update the state
    mapInfo = get(handles.axesMap, 'UserData');
    mapInfo.state = 'ObstacleCircle';
    set(handles.axesMap, 'UserData', mapInfo);
    
    % Set 'button down' function of axesMap
    set(handles.axesMap, 'ButtonDownFcn', {@cbaddobstaclecircle});

% --------------------------------------------------------------------
function tbppDelete_OnCallback(hObject, eventdata, handles)
% hObject    handle to tbppDelete (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Update the state
    mapInfo = get(handles.axesMap, 'UserData');
    mapInfo.state = 'Delete';
    set(handles.axesMap, 'UserData', mapInfo);
    
    % Set 'button down' function of axesMap
    set(handles.axesMap, 'ButtonDownFcn', {@cbdeleteobstacle})

% --------------------------------------------------------------------
function tbppSelect_OffCallback(hObject, eventdata, handles)
% hObject    handle to tbppSelect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    mapInfo = get(handles.axesMap, 'UserData');
    if strcmp(mapInfo.state, 'Select')
        set(hObject, 'State', 'on');
    end
    
% --------------------------------------------------------------------
function tbppSetStartPoint_OffCallback(hObject, eventdata, handles)
% hObject    handle to tbppSetStartPoint (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    mapInfo = get(handles.axesMap, 'UserData');
    if strcmp(mapInfo.state, 'StartPoint')
        set(hObject, 'State', 'on');
    end
    
% --------------------------------------------------------------------
function tbppAddWaypoint_OffCallback(hObject, eventdata, handles)
% hObject    handle to tbppAddWaypoint (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    mapInfo = get(handles.axesMap, 'UserData');
    if strcmp(mapInfo.state, 'Waypoint')
        set(hObject, 'State', 'on');
    end

% --------------------------------------------------------------------
function tbppAddObstacleCircle_OffCallback(hObject, eventdata, handles)
% hObject    handle to tbppAddObstacleCircle (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    mapInfo = get(handles.axesMap, 'UserData');
    if strcmp(mapInfo.state, 'ObstacleCircle')
        set(hObject, 'State', 'on');
    end

% --------------------------------------------------------------------
function tbppDelete_OffCallback(hObject, eventdata, handles)
% hObject    handle to tbppDelete (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    mapInfo = get(handles.axesMap, 'UserData');
    if strcmp(mapInfo.state, 'Delete')
        set(hObject, 'State', 'on');
    end
    
% --------------------------------------------------------------------
function tbppRunPathSimulation_ClickedCallback(hObject, eventdata, handles)
% hObject    handle to tbppRunPathSimulation (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % ===== Check if minimum simulation requirements are met =====
    mapInfo = get(handles.axesMap, 'UserData');
    if isempty(mapInfo.startpoint)
       return
    end
    
    if isempty(mapInfo.waypoints)
        return
    end
    
    % ============================================================
        
    % Execute path planning simulation
    runsim(handles);
    
    % Disable simulation tool (reset will re-enable this)
    set(hObject, 'Enable', 'off');
    
    % Enable reset tool
    set(handles.tbppReset, 'Enable', 'on');

% --------------------------------------------------------------------
function tbppReset_ClickedCallback(hObject, eventdata, handles)
% hObject    handle to tbppReset (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % ===== Remove simulation objects =====
    mapInfo = get(handles.axesMap, 'UserData');
    
    % Delete paths
    delete(mapInfo.paths);
    mapInfo.paths = [];
    
    % Delete free configuration points
    delete(mapInfo.cFree);
    mapInfo.cFree = [];
    
    % Store changes to the map object
    set(handles.axesMap, 'UserData', mapInfo);
    
    % =====================================
    
    % Re-enable simulation tool
    set(handles.tbppRunPathSimulation, 'Enable', 'on');
    
    % Disable reset tool
    set(hObject, 'Enable', 'off')
    

% --------------------------------------------------------------------
function tbppAddObstacleRect_ClickedCallback(hObject, eventdata, handles)
% hObject    handle to tbppAddObstacleRect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Disable other path planning toggle tools
    set(handles.tbppSetStartPoint, 'State', 'off');
    set(handles.tbppAddWaypoint, 'State', 'off');
    set(handles.tbppAddObstacleCircle, 'State', 'off');
    set(handles.tbppSelect, 'State', 'off');
    set(handles.tbppDelete, 'State', 'off');

% --------------------------------------------------------------------
function tbppAddObstacleRect_OffCallback(hObject, eventdata, handles)
% hObject    handle to tbppAddObstacleRect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    mapInfo = get(handles.axesMap, 'UserData');
    if strcmp(mapInfo.state, 'ObstacleRect')
        set(hObject, 'State', 'on');
    end
    
% --------------------------------------------------------------------
function tbppAddObstacleRect_OnCallback(hObject, eventdata, handles)
% hObject    handle to tbppAddObstacleRect (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Update the state
    mapInfo = get(handles.axesMap, 'UserData');
    mapInfo.state = 'ObstacleRect';
    set(handles.axesMap, 'UserData', mapInfo);
    
    % Set 'button down' function of axesMap
    set(handles.axesMap, 'ButtonDownFcn', {@cbaddobstaclerect, handles});


% --- Executes on button press in pbWaypointUp.
function pbWaypointUp_Callback(hObject, eventdata, handles)
% hObject    handle to pbWaypointUp (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    iName = get(handles.lbWaypoints, 'Value');
    if iName > 1
        iNamePrev  = iName - 1;
        
        % Move the waypoint name up in the list box
        waypointNames = get(handles.lbWaypoints, 'String');
        tmp = waypointNames{iNamePrev};
        waypointNames{iNamePrev} = waypointNames{iName};
        waypointNames{iName} = tmp;
        set(handles.lbWaypoints, 'String', waypointNames);
        
        % Move the waypoint up in the list of waypoint handles
        mapInfo = get(handles.axesMap, 'UserData');
        tmp = mapInfo.waypoints(iNamePrev);
        mapInfo.waypoints(iNamePrev) = mapInfo.waypoints(iName);
        mapInfo.waypoints(iName) = tmp;
        set(handles.axesMap, 'UserData', mapInfo);
        
        % Set the new selected value
        set(handles.lbWaypoints, 'Value', iNamePrev);
    end

% --- Executes on button press in pbWaypointDown.
function pbWaypointDown_Callback(hObject, eventdata, handles)
% hObject    handle to pbWaypointDown (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    iName = get(handles.lbWaypoints, 'Value');
    waypointNames = get(handles.lbWaypoints, 'String');
    if iName < size(waypointNames, 1);
        iNameNext  = iName + 1;
        
        % Move the waypoint name down in the list box
        tmp = waypointNames{iNameNext};
        waypointNames{iNameNext} = waypointNames{iName};
        waypointNames{iName} = tmp;
        set(handles.lbWaypoints, 'String', waypointNames);
        
        % Move the waypoint up in the list of waypoint handles
        mapInfo = get(handles.axesMap, 'UserData');
        tmp = mapInfo.waypoints(iNameNext);
        mapInfo.waypoints(iNameNext) = mapInfo.waypoints(iName);
        mapInfo.waypoints(iName) = tmp;
        set(handles.axesMap, 'UserData', mapInfo);
        
        % Set the new selected value
        set(handles.lbWaypoints, 'Value', iNameNext);
    end


% --- Executes on button press in pbWaypointDelete.
function pbWaypointDelete_Callback(hObject, eventdata, handles)
% hObject    handle to pbWaypointDelete (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    iName = get(handles.lbWaypoints, 'Value');
    
    % Remove waypoint name from listbox
    waypointNames = get(handles.lbWaypoints, 'String');
    waypointNames(iName) = [];
    set(handles.lbWaypoints, 'String', waypointNames);

    % Remove waypoint in the list of waypoint object handles
    mapInfo = get(handles.axesMap, 'UserData');
    hWaypoint = mapInfo.waypoints(iName);
    mapInfo.waypoints(iName) = [];
    set(handles.axesMap, 'UserData', mapInfo);
    
    % Delete waypoint
    delete(hWaypoint);
    
    % Set value
    nWaypoints = size(waypointNames, 1);
    value = nWaypoints;
    set(handles.lbWaypoints, 'Value', value);
    
% --------------------------------------------------------------------
function menuFile_Callback(hObject, eventdata, handles)
% hObject    handle to menuFile (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% --------------------------------------------------------------------
function menuFileLoadMap_Callback(hObject, eventdata, handles)
% hObject    handle to menuFileLoadMap (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% --------------------------------------------------------------------
function menuFileSaveMap_Callback(hObject, eventdata, handles)
% hObject    handle to menuFileSaveMap (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% --------------------------------------------------------------------
function menuEdit_Callback(hObject, eventdata, handles)
% hObject    handle to menuEdit (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% --------------------------------------------------------------------
function menuEditDeleteStartPoint_Callback(hObject, eventdata, handles)
% hObject    handle to menuEditDeleteStartPoint (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Delete the startpoint
    mapInfo = get(handles.axesMap, 'UserData');
    h = mapInfo.startpoint;
    delete(h);
    mapInfo.startpoint = [];
    
    % Disable this option
    set(hObject, 'Enable', 'off');
    
    % Enable start point option
    set(handles.tbppSetStartPoint, 'Enable', 'on');

    



function editGap_Callback(hObject, eventdata, handles)
% hObject    handle to editGap (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

% Hints: get(hObject,'String') returns contents of editGap as text
%        str2double(get(hObject,'String')) returns contents of editGap as a double


% --- Executes during object creation, after setting all properties.
function editGap_CreateFcn(hObject, eventdata, handles)
% hObject    handle to editGap (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    empty - handles not created until after all CreateFcns called

% Hint: edit controls usually have a white background on Windows.
%       See ISPC and COMPUTER.
if ispc && isequal(get(hObject,'BackgroundColor'), get(0,'defaultUicontrolBackgroundColor'))
    set(hObject,'BackgroundColor','white');
end
