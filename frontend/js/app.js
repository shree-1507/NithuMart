const API = "http://127.0.0.1:8080/api";


// ===============================
// REGISTER
// ===============================

const registerForm = document.getElementById("registerForm");

if (registerForm) {

    registerForm.addEventListener("submit", async function (event) {

        event.preventDefault();

        const name = document.getElementById("name").value;
        const email = document.getElementById("email").value;
        const password = document.getElementById("password").value;
        const role = document.getElementById("role").value;

        try {

            const response = await fetch(`${API}/auth/register`, {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify({
                    name: name,
                    email: email,
                    password: password,
                    role: role
                })
            });

            const data = await response.json();

            document.getElementById("message").innerText =
                data.message || "Registration successful";

            if (response.ok) {
                setTimeout(function () {
                    window.location.href = "login.html";
                }, 1000);
            }

        } catch (error) {

           

    console.error(error);

    document.getElementById("message").innerText =
        "Error: " + error.message;


        }

    });

}


// ===============================
// LOGIN
// ===============================

const loginForm = document.getElementById("loginForm");

if (loginForm) {

    loginForm.addEventListener("submit", async function (event) {

        event.preventDefault();

        const email = document.getElementById("email").value;
        const password = document.getElementById("password").value;

        try {

            const response = await fetch(`${API}/auth/login`, {
                method: "POST",
                headers: {
                    "Content-Type": "application/json"
                },
                body: JSON.stringify({
                    email: email,
                    password: password
                })
            });

            const data = await response.json();

            if (!response.ok) {

                document.getElementById("message").innerText =
                    data.message || "Invalid login";

                return;
            }

            /*
               Save the logged-in user.

               The backend response may contain
               the user directly or inside "user".
            */

            const user = data.user || data;

            localStorage.setItem("user", JSON.stringify(user));

            document.getElementById("message").innerText =
                "Login successful!";

            setTimeout(function () {

                if (user.role === "Seller") {
                    window.location.href = "seller.html";
                }
                else if (user.role === "Admin") {
                    window.location.href = "admin.html";
                }
                else {
                    window.location.href = "products.html";
                }

            }, 800);

        } catch (error) {

            document.getElementById("message").innerText =
                "Unable to connect to server.";

        }

    });

}


// ===============================
// LOAD PRODUCTS
// ===============================

async function loadProducts() {

    const productList = document.getElementById("productList");

    if (!productList) {
        return;
    }

    try {

        const response = await fetch(`${API}/products`);

        const data = await response.json();

        let products = Array.isArray(data)
            ? data
            : data.products || [];

        const searchText =
            document.getElementById("searchInput")?.value
            .toLowerCase() || "";

        const category =
            document.getElementById("categoryFilter")?.value || "";

        products = products.filter(function (product) {

            const name =
                String(product.name || "").toLowerCase();

            const productCategory =
                String(product.category || "");

            const matchesSearch =
                name.includes(searchText);

            const matchesCategory =
                category === "" ||
                productCategory === category;

            return matchesSearch && matchesCategory;

        });


        if (products.length === 0) {

            productList.innerHTML =
                "<p>No products found.</p>";

            return;
        }


        productList.innerHTML = "";


        products.forEach(function (product) {

            const card = document.createElement("div");

            card.className = "product-card";

            card.innerHTML = `
                <h3>${product.name}</h3>

                <p>${product.description || ""}</p>

                <p><b>Category:</b> ${product.category}</p>

                <p><b>Price:</b> ₹${product.price}</p>

                <p><b>Stock:</b> ${product.stock}</p>

                <button onclick="addToCart(${product.id})">
                    Add to Cart
                </button>
            `;

            productList.appendChild(card);

        });

    } catch (error) {

        productList.innerHTML =
            "<p>Unable to load products.</p>";

    }

}


// ===============================
// ADD TO CART
// ===============================

async function addToCart(productId) {

    const user = JSON.parse(
        localStorage.getItem("user")
    );

    if (!user) {

        alert("Please login first.");

        window.location.href = "login.html";

        return;
    }

    const buyerId =
        user.id || user.user_id || user.buyer_id;


    try {

        const response = await fetch(`${API}/cart`, {

            method: "POST",

            headers: {
                "Content-Type": "application/json"
            },

            body: JSON.stringify({
                buyer_id: buyerId,
                product_id: productId,
                quantity: 1
            })

        });


        const data = await response.json();

        alert(
            data.message ||
            "Product added to cart"
        );

    } catch (error) {

        alert("Unable to add product to cart.");

    }

}


// ===============================
// LOAD CART
// ===============================

async function loadCart() {

    const cartList =
        document.getElementById("cartList");

    if (!cartList) {
        return;
    }


    const user = JSON.parse(
        localStorage.getItem("user")
    );


    if (!user) {

        cartList.innerHTML =
            "<p>Please login to view your cart.</p>";

        return;
    }


    const buyerId =
        user.id || user.user_id || user.buyer_id;


    try {

        const response =
            await fetch(`${API}/cart/${buyerId}`);

        const data =
            await response.json();

        const items =
            Array.isArray(data)
                ? data
                : data.cart || data.items || [];


        if (items.length === 0) {

            cartList.innerHTML =
                "<p>Your cart is empty.</p>";

            document.getElementById("cartTotal")
                .innerText = "Total: ₹0";

            return;
        }


        cartList.innerHTML = "";

        let total = 0;


        items.forEach(function (item) {

            const price =
                Number(item.price || item.product_price || 0);

            const quantity =
                Number(item.quantity || 1);

            total += price * quantity;


            const div =
                document.createElement("div");

            div.className = "product-card";


            div.innerHTML = `
                <h3>${item.name || item.product_name}</h3>

                <p>Price: ₹${price}</p>

                <p>Quantity: ${quantity}</p>

                <p>Subtotal: ₹${price * quantity}</p>

                <button onclick="removeCartItem(${item.id})">
                    Remove
                </button>
            `;


            cartList.appendChild(div);

        });


        document.getElementById("cartTotal")
            .innerText = `Total: ₹${total}`;

    } catch (error) {

        cartList.innerHTML =
            "<p>Unable to load cart.</p>";

    }

}


// ===============================
// REMOVE CART ITEM
// ===============================

async function removeCartItem(cartId) {

    try {

        const response =
            await fetch(`${API}/cart/${cartId}`, {
                method: "DELETE"
            });


        const data =
            await response.json();

        alert(
            data.message ||
            "Item removed from cart"
        );

        loadCart();

    } catch (error) {

        alert("Unable to remove item.");

    }

}


// ===============================
// CHECKOUT
// ===============================

async function checkout() {

    const user = JSON.parse(
        localStorage.getItem("user")
    );


    if (!user) {

        alert("Please login first.");

        window.location.href = "login.html";

        return;
    }


    const buyerId =
        user.id || user.user_id || user.buyer_id;


    try {

        const response =
            await fetch(`${API}/orders`, {

                method: "POST",

                headers: {
                    "Content-Type": "application/json"
                },

                body: JSON.stringify({
                    buyer_id: buyerId
                })

            });


        const data =
            await response.json();


        if (response.ok) {

            document.getElementById("cartMessage")
                .innerText =
                data.message ||
                "Order placed successfully";


            setTimeout(function () {
                window.location.href = "orders.html";
            }, 1000);

        }
        else {

            document.getElementById("cartMessage")
                .innerText =
                data.message ||
                "Unable to place order.";

        }

    } catch (error) {

        document.getElementById("cartMessage")
            .innerText =
            "Unable to connect to server.";

    }

}


// ===============================
// LOAD BUYER ORDERS
// ===============================

async function loadOrders() {

    const orderList =
        document.getElementById("orderList");

    if (!orderList) {
        return;
    }


    const user = JSON.parse(
        localStorage.getItem("user")
    );


    if (!user) {

        orderList.innerHTML =
            "<p>Please login to view orders.</p>";

        return;
    }


    const buyerId =
        user.id || user.user_id || user.buyer_id;


    try {

        const response =
            await fetch(`${API}/orders/buyer/${buyerId}`);

        const data =
            await response.json();


        const orders =
            Array.isArray(data)
                ? data
                : data.orders || [];


        if (orders.length === 0) {

            orderList.innerHTML =
                "<p>No orders found.</p>";

            return;
        }


        orderList.innerHTML = "";


        orders.forEach(function (order) {

            const div =
                document.createElement("div");

            div.className = "product-card";


            div.innerHTML = `
                <h3>Order #${order.id}</h3>

                <p>
                    <b>Total:</b>
                    ₹${order.total_amount || order.total || 0}
                </p>

                <p>
                    <b>Status:</b>
                    ${order.status || "Pending"}
                </p>
            `;


            orderList.appendChild(div);

        });

    } catch (error) {

        orderList.innerHTML =
            "<p>Unable to load orders.</p>";

    }

}


// ===============================
// SELLER - ADD PRODUCT
// ===============================

const productForm =
    document.getElementById("productForm");


if (productForm) {

    productForm.addEventListener(
        "submit",
        async function (event) {

            event.preventDefault();


            const user = JSON.parse(
                localStorage.getItem("user")
            );


            if (!user) {

                alert("Please login first.");

                return;
            }


            const sellerId =
                user.id ||
                user.user_id ||
                user.seller_id;


            const body = {

                seller_id: sellerId,

                name:
                    document.getElementById(
                        "productName"
                    ).value,

                description:
                    document.getElementById(
                        "productDescription"
                    ).value,

                price:
                    Number(
                        document.getElementById(
                            "productPrice"
                        ).value
                    ),

                category:
                    document.getElementById(
                        "productCategory"
                    ).value,

                stock:
                    Number(
                        document.getElementById(
                            "productStock"
                        ).value
                    )

            };


            try {

                const response =
                    await fetch(`${API}/products`, {

                        method: "POST",

                        headers: {
                            "Content-Type":
                                "application/json"
                        },

                        body:
                            JSON.stringify(body)

                    });


                const data =
                    await response.json();


                document.getElementById(
                    "productMessage"
                ).innerText =
                    data.message ||
                    "Product added successfully";


                if (response.ok) {

                    productForm.reset();

                    loadSellerProducts();

                }

            } catch (error) {

                 console.error(error);

    document.getElementById(
        "productMessage"
    ).innerText =
        "Error: " + error.message;

            }

        }
    );

}


// ===============================
// SELLER PRODUCTS
// ===============================

async function loadSellerProducts() {

    const container =
        document.getElementById(
            "sellerProducts"
        );


    if (!container) {
        return;
    }


    try {

        const response =
            await fetch(`${API}/products`);

        const data =
            await response.json();


        const products =
            Array.isArray(data)
                ? data
                : data.products || [];


        const user =
            JSON.parse(
                localStorage.getItem("user")
            );


        const sellerId =
            user?.id ||
            user?.user_id ||
            user?.seller_id;


        const myProducts =
            products.filter(function (product) {

                return String(
                    product.seller_id
                ) === String(sellerId);

            });


        if (myProducts.length === 0) {

            container.innerHTML =
                "<p>No products added yet.</p>";

            return;
        }


        container.innerHTML = "";


        myProducts.forEach(function (product) {

            const div =
                document.createElement("div");

            div.className =
                "product-card";


            div.innerHTML = `
                <h3>${product.name}</h3>

                <p>Price: ₹${product.price}</p>

                <p>Category: ${product.category}</p>

                <p>Stock: ${product.stock}</p>

                <button onclick="deleteProduct(${product.id})">
                    Delete
                </button>
            `;


            container.appendChild(div);

        });

    } catch (error) {

        container.innerHTML =
            "<p>Unable to load products.</p>";

    }

}


// ===============================
// DELETE SELLER PRODUCT
// ===============================

async function deleteProduct(productId) {

    if (!confirm(
        "Are you sure you want to delete this product?"
    )) {
        return;
    }


    try {

        const response =
            await fetch(
                `${API}/products/${productId}`,
                {
                    method: "DELETE"
                }
            );


        const data =
            await response.json();


        alert(
            data.message ||
            "Product deleted successfully"
        );


        loadSellerProducts();

    } catch (error) {

        alert("Unable to delete product.");

    }

}


// ===============================
// SELLER ORDERS
// ===============================

async function loadSellerOrders() {

    const container =
        document.getElementById(
            "sellerOrders"
        );


    if (!container) {
        return;
    }


    const user =
        JSON.parse(
            localStorage.getItem("user")
        );


    if (!user) {

        container.innerHTML =
            "<p>Please login first.</p>";

        return;
    }


    const sellerId =
        user.id ||
        user.user_id ||
        user.seller_id;


    try {

        const response =
            await fetch(
                `${API}/orders/seller/${sellerId}`
            );


        const data =
            await response.json();


        const orders =
            Array.isArray(data)
                ? data
                : data.orders || [];


        if (orders.length === 0) {

            container.innerHTML =
                "<p>No received orders.</p>";

            return;
        }


        container.innerHTML = "";


        orders.forEach(function (order) {

            const div =
                document.createElement("div");

            div.className =
                "product-card";


            div.innerHTML = `
                <h3>Order #${order.id}</h3>

                <p>
                    Buyer ID:
                    ${order.buyer_id || "-"}
                </p>

                <p>
                    Total:
                    ₹${order.total_amount || 0}
                </p>

                <p>
                    Status:
                    ${order.status || "Pending"}
                </p>
            `;


            container.appendChild(div);

        });

    } catch (error) {

        container.innerHTML =
            "<p>Unable to load orders.</p>";

    }

}


// ===============================
// ADMIN USERS
// ===============================

async function loadAdminUsers() {

    const container =
        document.getElementById(
            "userList"
        );


    if (!container) {
        return;
    }


    try {

        const response =
            await fetch(`${API}/admin/users`);


        const data =
            await response.json();


        const users =
            Array.isArray(data)
                ? data
                : data.users || [];


        if (users.length === 0) {

            container.innerHTML =
                "<p>No users found.</p>";

            return;
        }


        container.innerHTML = "";


        users.forEach(function (user) {

            const div =
                document.createElement("div");

            div.className =
                "product-card";


            div.innerHTML = `
                <p><b>ID:</b> ${user.id}</p>

                <p><b>Name:</b> ${user.name}</p>

                <p><b>Email:</b> ${user.email}</p>

                <p><b>Role:</b> ${user.role}</p>
            `;


            container.appendChild(div);

        });

    } catch (error) {

        container.innerHTML =
            "<p>Unable to load users.</p>";

    }

}


// ===============================
// ADMIN ORDERS
// ===============================

async function loadAdminOrders() {

    const container =
        document.getElementById(
            "adminOrderList"
        );


    if (!container) {
        return;
    }


    try {

        const response =
            await fetch(`${API}/admin/orders`);


        const data =
            await response.json();


        const orders =
            Array.isArray(data)
                ? data
                : data.orders || [];


        if (orders.length === 0) {

            container.innerHTML =
                "<p>No orders found.</p>";

            return;
        }


        container.innerHTML = "";


        orders.forEach(function (order) {

            const div =
                document.createElement("div");

            div.className =
                "product-card";


            div.innerHTML = `
                <h3>Order #${order.id}</h3>

                <p>
                    Buyer ID:
                    ${order.buyer_id || "-"}
                </p>

                <p>
                    Total:
                    ₹${order.total_amount || 0}
                </p>

                <p>
                    Status:
                    ${order.status || "Pending"}
                </p>
            `;


            container.appendChild(div);

        });

    } catch (error) {

        container.innerHTML =
            "<p>Unable to load orders.</p>";

    }

}


// ===============================
// ADMIN PRODUCTS
// ===============================

async function loadAdminProducts() {

    const container =
        document.getElementById(
            "adminProductList"
        );


    if (!container) {
        return;
    }


    try {

        const response =
            await fetch(`${API}/products`);


        const data =
            await response.json();


        const products =
            Array.isArray(data)
                ? data
                : data.products || [];


        if (products.length === 0) {

            container.innerHTML =
                "<p>No products found.</p>";

            return;
        }


        container.innerHTML = "";


        products.forEach(function (product) {

            const div =
                document.createElement("div");

            div.className =
                "product-card";


            div.innerHTML = `
                <h3>${product.name}</h3>

                <p>Seller ID: ${product.seller_id}</p>

                <p>Price: ₹${product.price}</p>

                <p>Category: ${product.category}</p>

                <p>Stock: ${product.stock}</p>

                <button onclick="adminDeleteProduct(${product.id})">
                    Remove Product
                </button>
            `;


            container.appendChild(div);

        });

    } catch (error) {

        container.innerHTML =
            "<p>Unable to load products.</p>";

    }

}


// ===============================
// ADMIN DELETE PRODUCT
// ===============================

async function adminDeleteProduct(productId) {

    if (!confirm(
        "Remove this product?"
    )) {
        return;
    }


    try {

        const response =
            await fetch(
                `${API}/admin/products/${productId}`,
                {
                    method: "DELETE"
                }
            );


        const data =
            await response.json();


        alert(
            data.message ||
            "Product removed successfully"
        );


        loadAdminProducts();

    } catch (error) {

        alert(
            "Unable to remove product."
        );

    }

}