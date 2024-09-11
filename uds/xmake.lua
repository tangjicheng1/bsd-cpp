set_project("uds_example")

add_rules("mode.debug", "mode.release")

target("server1")
    set_kind("binary")           
    add_files("server1.cpp")     
    set_targetdir("$(buildir)") 

target("uds_client")
    set_kind("binary")      
    add_files("client1.cpp")    
    set_targetdir("$(buildir)")  
