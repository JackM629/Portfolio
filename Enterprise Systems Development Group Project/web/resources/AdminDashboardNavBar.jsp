<%-- 
    Document   : AdminDashboardNavBar
    Created on : 27-Nov-2017, 13:16:36
    Author     : Frazer, Sasha
--%>
<div class="navBar">
    <ul class="menu">
        <li><a href="${pageContext.request.contextPath}/docs/admin/AdminDashboard">Home</a></li>
        <li><a href="${pageContext.request.contextPath}/docs/admin/ListMembers">Members</a></li>
        <li><a href="${pageContext.request.contextPath}/docs/admin/ListBalance">Balances</a></li>
        <li><a href="${pageContext.request.contextPath}/docs/admin/ListClaims">Claims</a></li>
        <li><a href="${pageContext.request.contextPath}/docs/admin/ListApplications">Applications</a></li>
        <li><a href="${pageContext.request.contextPath}/docs/admin/ManageTurnover">Turnover</a></li>
    </ul>
</div>
<div class="logout">
    <form class="form" action="${pageContext.request.contextPath}/Logout" method="post">
        <input class="logoutBtn" type="submit" value="Logout" >
    </form>
</div>
        
