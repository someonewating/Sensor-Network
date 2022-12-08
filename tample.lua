--You can define your own functions as is done here for parsePayload
function parsePayload(appeui,deveui,payload)
	Tag1 = "light"
	strvalue = resiot_hexdecode_ascii(payload)
	--Call for LUA Script engine prints
	resiot_debug(string.format("PL TEST Tag: %s  Strvalue: %s \n",Tag1, strvalue))
    value = string.match(strvalue, 'Light Sensor Value is ([0-9]+)')
	resiot_debug(string.format("PL TEST Tag: %s  Value: %s \n",Tag1, value))
	worked, err = resiot_setnodevalue(appeui, deveui, Tag1, value)
	if (not worked) then
		resiot_debug(string.format("PL TEST Set Value Error %s \n",err)) 
	else
		resiot_debug("PL TEST Set Node value successful\n")
	end  
end

Origin = resiot_startfrom() --Scene process starts here


--Manual script execution for testing
if Origin == "Manual" then 
	payload = "4c696768742053656e736f722056616c75652069732034" --Set your test payload here in hexadecimal
	appeui = "70b3d57ed000fc4d" --Set your Application EUI here
	deveui = "7E39323559379194" --Set your own Device EUI here
-- Normal execution, get payload received from device
else 
	appeui = resiot_comm_getparam("appeui")
	deveui = resiot_comm_getparam("deveui")
	payload, err = resiot_getlastpayload(appeui, deveui)
  	resiot_debug("PL TEST Test Auto Mode\n")
  
   
end
--Do your stuff
parsePayload(appeui,deveui,payload)