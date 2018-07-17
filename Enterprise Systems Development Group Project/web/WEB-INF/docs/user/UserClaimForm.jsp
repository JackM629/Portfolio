<%-- 
    Document   : UserClaimForm
    Created on : 27-Nov-2017, 13:16:36
    Author     : Frazer, Sasha
--%>

<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ include file="/resources/UserDashboardNavBar.jsp" %>
<div class="main">
    <div class="content-screen">
        <div class="title">
            <h1>Make Claim</h1>
        </div>
        <form class="form" action="${pageContext.request.contextPath}/UserController" method="post" style="text-align: center;">
            <div class="input-label">
                <label>  Claim Amount</label>
            </div>
            <input class="control-group" type="number" name="amount">
           <div class="input-label">
                <label> Claim Rationale</label>
            </div>
            <input class="control-group" type="text" name="rationale" maxlength="10">
            <input type="hidden" name="viewId" value="/UserMakeClaim">
            <input type="hidden" name="username" value="${sessionScope.username}">
            <input class="btn" type="submit" value="Submit Claim"/>
            <input class="btn" type="reset" value="Reset">
        </form>
    </div>
</div>