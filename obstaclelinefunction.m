% Obstacle function:
% Author: K. Passino, Version: 1/25/01
function J=obstaclelinefunction(x,w1)

% An example function to represent sensed obstacles:

c=2;

	J=...
		w1*max([exp(-lineDistance([10 15]',[10 25]',x)^2*0.8),...
                exp(-lineDistance([10; 15], [25; 15],x)^2*0.8),...
		        exp(-0.8*((x(1,1)- 8)^2 + (x(2,1)-10)^2)),...
		        exp(-0.8*((x(1,1)-10)^2 + (x(2,1)-10)^2)),...
		        exp(-0.8*((x(1,1)-12)^2 + (x(2,1)- 8)^2)),...
		        exp(-0.8*((x(1,1)-12)^2 + (x(2,1)- 6)^2)),...
		        exp(-0.8*((x(1,1)-24)^2 + (x(2,1)-20)^2)),...
		        exp(-0.8*((x(1,1)-18)^2 + (x(2,1)-20)^2))]);


            
end

%  http://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
% float minimum_distance(vec2 v, vec2 w, vec2 p) {
% %   // Return minimum distance between line segment vw and point p
%   const float l2 = length_squared(v, w);  // i.e. |w-v|^2 -  avoid a sqrt
%   if (l2 == 0.0) return distance(p, v);   // v == w case
% %   // Consider the line extending the segment, parameterized as v + t (w - v).
% %   // We find projection of point p onto the line. 
% %   // It falls where t = [(p-v) . (w-v)] / |w-v|^2
%   const float t = dot(p - v, w - v) / l2;
%   if (t < 0.0) return distance(p, v);       // Beyond the 'v' end of the segment
%   else if (t > 1.0) return distance(p, w);  // Beyond the 'w' end of the segment
%   const vec2 projection = v + t * (w - v);  // Projection falls on the segment
%   return distance(p, projection);
% }
