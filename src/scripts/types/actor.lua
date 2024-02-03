require("types.form")

function actor.getplayer()
	return actor.new(form.lookupbyid(0x14))
end