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

% Last Modified by GUIDE v2.5 09-Apr-2012 17:55:09

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

% 'Global' Variables
% Map information
% Map state (Select, StartPoint, Waypoint, ObstacleCircle, ObstacleLine, Delete)
mapInfo.state = 'Select';
mapInfo.startpoint = [];
mapInfo.waypoints = [];
mapInfo.lastWaypointIndex = 1;
mapInfo.boundaries = [];
mapInfo.obstacleCircles = [];
mapInfo.obstacleLines = [];
set(handles.axesMap, 'UserData', mapInfo);
set(handles.axesMap, 'ButtonDownFcn', []);

% Add fixed boundaries
addboundaries(handles.axesMap);

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
    set(handles.tbppAddObstacleLine, 'State', 'off');
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
    set(handles.tbppAddObstacleLine, 'State', 'off');
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
    set(handles.tbppAddObstacleLine, 'State', 'off');
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
    set(handles.tbppAddObstacleLine, 'State', 'off');
    set(handles.tbppDelete, 'State', 'off');

% --------------------------------------------------------------------
function tbppAddObstacleLine_ClickedCallback(hObject, eventdata, handles)
% hObject    handle to tbppAddObstacleLine (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Disable other path planning toggle tools
    set(handles.tbppSetStartPoint, 'State', 'off');
    set(handles.tbppSelect, 'State', 'off');
    set(handles.tbppAddWaypoint, 'State', 'off');
    set(handles.tbppAddObstacleCircle, 'State', 'off');
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
    set(handles.tbppAddObstacleLine, 'State', 'off');
    
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
function tbppAddObstacleLine_OnCallback(hObject, eventdata, handles)
% hObject    handle to tbppAddObstacleLine (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    % Update the state
    mapInfo = get(handles.axesMap, 'UserData');
    mapInfo.state = 'ObstacleLine';
    set(handles.axesMap, 'UserData', mapInfo);
    
    % Set 'button down' function of axesMap
    set(handles.axesMap, 'ButtonDownFcn', {@cbaddobstacleline});

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
    set(handles.axesMap, 'ButtonDownFcn', []);

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
function tbppAddObstacleLine_OffCallback(hObject, eventdata, handles)
% hObject    handle to tbppAddObstacleLine (see GCBO)
% eventdata  reserved - to be defined in a future version of MATLAB
% handles    structure with handles and user data (see GUIDATA)

    mapInfo = get(handles.axesMap, 'UserData');
    if strcmp(mapInfo.state, 'ObstacleLine')
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

    % --- Check if all minimum requirements exist ---
    mapInfo = get(handles.axesMap, 'UserData');
    % TODO: add message box on error (i.e. no startpoint, no waypoint, etc)
    if isempty(mapInfo.startpoint)
        return;
    end
    
    if isempty(mapInfo.waypoints)
        return;
    end
    
    cbrunsim(handles.axesMap)
     
   
