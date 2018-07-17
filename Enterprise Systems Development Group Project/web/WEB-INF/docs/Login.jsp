<%-- 
    Document   : UserLogin
    Created on : 27-Nov-2017, 13:16:36
    Author     : Frazer, Sasha
--%>
<div class="main">
    <div class="content-screen">
        <div class="title">
            <h1>Login</h1>
        </div>
        <form class="form" name="login" action="${pageContext.request.contextPath}/Login" method="post">
            <div class="input-label">
                <label>Username</label>
            </div>
            <input class="control-group" type="text" name="username"  value="">
            <div class="input-label">
                <label>Password</label>
            </div>
            <input class="control-group" type="password" name="password" value="">
            <input class="btn" type="submit" value="Login">
            <input class="btn" type="reset" value="Reset">
            <!--Display the error message-->
            <c:if test="${not empty post}">
                ${ErrorMessage}
            </c:if>       
        </form>
    </div>
</div>