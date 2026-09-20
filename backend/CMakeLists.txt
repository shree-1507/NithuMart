cmake_minimum_required(VERSION 3.20)

project(NithuMartBackend)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Drogon CONFIG REQUIRED)

add_executable(NithuMartBackend

    main.cpp

    controllers/AuthController.cpp
    controllers/ProductController.cpp
    controllers/CartController.cpp
    controllers/OrderController.cpp
    controllers/AdminController.cpp
    controllers/ReviewController.cpp

    services/AuthService.cpp
    services/ProductService.cpp
    services/CartService.cpp
    services/OrderService.cpp
    services/AdminService.cpp
    services/ReviewService.cpp

    models/User.cpp
    models/Product.cpp
    models/CartItem.cpp
    models/Order.cpp
    models/OrderItem.cpp
    models/Review.cpp

    routes/Routes.cpp

    database/Database.cpp
)

target_include_directories(NithuMartBackend PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(NithuMartBackend PRIVATE
    Drogon::Drogon
)