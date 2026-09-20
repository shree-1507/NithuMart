#include "Routes.h"

#include "../controllers/AuthController.h"
#include "../controllers/ProductController.h"
#include "../controllers/CartController.h"
#include "../controllers/OrderController.h"
#include "../controllers/AdminController.h"
#include "../controllers/ReviewController.h"

#include <drogon/drogon.h>

void Routes::registerAll()
{
    auto &app = drogon::app();

    // Authentication
    app.registerHandler(
        "/api/auth/register",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback)
        {
            callback(
                AuthController::registerUser(req));
        },
        {drogon::Post}
    );

    app.registerHandler(
        "/api/auth/login",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback)
        {
            callback(
                AuthController::login(req));
        },
        {drogon::Post}
    );

    // Products
    app.registerHandler(
        "/api/products",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback)
        {
            callback(
                ProductController::getAll(req));
        },
        {drogon::Get}
    );

    app.registerHandler(
        "/api/products",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback)
        {
            callback(
                ProductController::add(req));
        },
        {drogon::Post}
    );

    app.registerHandler(
        "/api/products/{id}",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback,
           int id)
        {
            callback(
                ProductController::getOne(req, id));
        },
        {drogon::Get}
    );

    app.registerHandler(
        "/api/products/{id}",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback,
           int id)
        {
            callback(
                ProductController::update(req, id));
        },
        {drogon::Put}
    );

    app.registerHandler(
        "/api/products/{id}",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback,
           int id)
        {
            callback(
                ProductController::remove(req, id));
        },
        {drogon::Delete}
    );

    // Cart
    app.registerHandler(
        "/api/cart/{buyerId}",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback,
           int buyerId)
        {
            callback(
                CartController::get(req, buyerId));
        },
        {drogon::Get}
    );

    app.registerHandler(
        "/api/cart",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback)
        {
            callback(
                CartController::add(req));
        },
        {drogon::Post}
    );

    app.registerHandler(
        "/api/cart/{id}",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback,
           int id)
        {
            callback(
                CartController::update(req, id));
        },
        {drogon::Put}
    );

    app.registerHandler(
        "/api/cart/{id}",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback,
           int id)
        {
            callback(
                CartController::remove(req, id));
        },
        {drogon::Delete}
    );

    // Orders
    app.registerHandler(
        "/api/orders",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback)
        {
            callback(
                OrderController::place(req));
        },
        {drogon::Post}
    );

    app.registerHandler(
        "/api/orders/buyer/{buyerId}",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback,
           int buyerId)
        {
            callback(
                OrderController::buyerOrders(
                    req, buyerId));
        },
        {drogon::Get}
    );

    app.registerHandler(
        "/api/orders/seller/{sellerId}",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback,
           int sellerId)
        {
            callback(
                OrderController::sellerOrders(
                    req, sellerId));
        },
        {drogon::Get}
    );

    app.registerHandler(
        "/api/orders/{id}/status",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback,
           int id)
        {
            callback(
                OrderController::status(req, id));
        },
        {drogon::Put}
    );

    // Admin
    app.registerHandler(
        "/api/admin/users",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback)
        {
            callback(
                AdminController::users(req));
        },
        {drogon::Get}
    );

    app.registerHandler(
        "/api/admin/orders",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback)
        {
            callback(
                AdminController::orders(req));
        },
        {drogon::Get}
    );

    app.registerHandler(
        "/api/admin/products/{id}",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback,
           int id)
        {
            callback(
                AdminController::deleteProduct(
                    req, id));
        },
        {drogon::Delete}
    );

    // Reviews
    app.registerHandler(
        "/api/reviews",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback)
        {
            callback(
                ReviewController::add(req));
        },
        {drogon::Post}
    );

    app.registerHandler(
        "/api/reviews/{productId}",
        [](const drogon::HttpRequestPtr &req,
           std::function<void(
               const drogon::HttpResponsePtr &)> &&callback,
           int productId)
        {
            callback(
                ReviewController::get(
                    req, productId));
        },
        {drogon::Get}
    );
}