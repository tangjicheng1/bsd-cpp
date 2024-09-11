set_project("uds_example")

add_rules("mode.debug", "mode.release")

target("udp_server1")
    set_kind("binary")           
    add_files("udp_server1.cpp")     
    set_targetdir("$(buildir)") 

target("udp_client1")
    set_kind("binary")      
    add_files("udp_client1.cpp")    
    set_targetdir("$(buildir)")  

target("tcp_server1")
    set_kind("binary")           
    add_files("tcp_server1.cpp")     
    set_targetdir("$(buildir)") 

target("tcp_client1")
    set_kind("binary")      
    add_files("tcp_client1.cpp")    
    set_targetdir("$(buildir)")  

target("uds_server1")
    set_kind("binary")           
    add_files("uds_server1.cpp")     
    set_targetdir("$(buildir)") 

target("uds_client1")
    set_kind("binary")      
    add_files("uds_client1.cpp")    
    set_targetdir("$(buildir)")  
