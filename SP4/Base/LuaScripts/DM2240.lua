function SaveToLuaFile(fileDirectory, outputString, overwrite)
   print("SaveToLuaFile...")
   local f;						-- The file
   if overwrite == true then		-- Wipe the contents with new data
      f = assert(io.open(fileDirectory, "w"))
   elseif overwrite == false then	-- Append with new data
      f = assert(io.open(fileDirectory, "a"))
   end
   -- Write to the file
   f:write(outputString)
   -- Close the file
   f:close()
   print("OK")
end

function CalculateDistanceSquare(x1,y1,z1,x2,y2,z2)
	local distanceSquare = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1) + (z2 - z1)*(z2 - z1)
	print(distanceSquare)
	return distanceSquare
end

function GetMinMax(...)
	local minValue = -1
	local maxValue = -1
	local avgValue = -1
	local numValues = 0
	for i,v in ipairs(arg) do
		if minValue == -1 then
			minValue = v
		elseif v < minValue then
			minValue = v
		end

		if maxValue == -1 then
			maxValue = v
		elseif v > maxValue then
			maxValue = v
		end
		
		avgValue = avgValue + v
		numValues = numValues + 1
    end
	avgValue = avgValue / numValues
	print(minValue, maxValue, avgValue, numValues)
	return minValue, maxValue, avgValue, numValues
end

title = "DM2240 - Week 14 Scripting"
width = 800
height = 600