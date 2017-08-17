-- implementing Example 6: Dragon Curve from
-- https://en.wikipedia.org/wiki/L-system
--
-- variables: X Y
--   X: X+YF+
--   Y: -FX-Y
--
-- constants: F + -
--   F: more forward
--   +: turn right
--   -: turn left

-- how many iterations to run
iterations = 10

-- start command string
start = "FX"

-- rules command characters: commands to run next iteration
rules = {}
rules["X"] = "X+YF+"
rules["Y"] = "-FX-Y"

-- constant command characters: define actions to perform
constants = {}
constants["F"] = function()
	-- forward
	local oldpos = pos
	pos = pos + vel
	of.drawLine(oldpos, pos)
end
constants["+"] = function()
	-- turn right
	vel:rotate(0, 0, angle)
end
constants["-"] = function()
	-- turn left
	vel:rotate(0, 0, -angle)
end

-- start pos & vel for distance & direction
startpos = of.Point(of.getWidth()*0.3, of.getHeight()*0.6)
startvel = of.Point(0, -10)

-- current pos & vel
pos = of.Point(startpos.x, startpos.y)
vel = of.Point(startvel.x, startvel.y)

-- how much to turn left or right
angle = 90

-- fbo to draw into
fbo = nil

-- has the system been generated?
generated = false

------------
-- Main Loop
------------

function setup()
	-- allocate fbo
	fbo = of.Fbo()
	fbo:allocate(of.getWidth(), of.getHeight())
	of.background(0)
end

function update()
end

function draw()

	-- draw the L-System into the fbo
	if not generated then
		of.setColor(255)
		of.noFill()
		fbo:beginFbo()
		-- run for specified amount of iterations
		local commands = start
		for i=0,iterations do
			local newcommands = ""
			-- read through the command string, char by char
			--print(i..": "..commands)
			local len = string.len(commands)
			for c=1,len do
				-- is current char in the command string a rule or a constant?
				local func = string.sub(commands, c, c)
				if rules[func] then
					-- a rule, append to next iterations command string
					newcommands = newcommands..rules[func]
				elseif constants[func] then
					-- a constant, run the function then append
					constants[func]()
					newcommands = newcommands..func
				else
					print("unknown char: "..func)
				end
			end
			commands = newcommands
			-- reset
			pos = of.Point(startpos.x, startpos.y)
			vel = of.Point(startvel.x, startvel.y)
		end
		fbo:endFbo()
		generated = true
	end

	-- draw the generated L-System output
	of.setColor(255)
	fbo:draw(0, 0)
end
